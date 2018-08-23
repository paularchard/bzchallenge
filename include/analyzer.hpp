// Copyright (C) 2018 Paul Archard
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License, version 3,
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <map>
#include <common.hpp>
#include <stdexcept>

// This class represents an "analyzer" that, when supplied with a set of
// date/value pairs, provides statistical values for the data. These include
// the mean, median and standard deviation.
// Since these values have some interdependecies (e.g. standard deviation
// is dependent on the mean value), the values are computed as a group,
// using whatever optimizations are available.
//
// The accessor methods of this class will throw a std::domain_error
// exception when called on an invald data set, i.e. if the data is empty,
// or the standard deviation is requested for data with less than two values
// (we are using "sample standard deviation" in this case).
//
class analyzer
{
public:
    analyzer(const std::map<date, double> &data);

    // return the number of samples in the data
    size_t count(void) const { return len; }
    
    // return the most recent pair with the highest value
    std::pair<date, double> highest(void) const;

    // return the most recent pair with the lowest value
    std::pair<date, double> lowest(void) const;

    // return the mean value of the data
    double mean_value(void) const;

    // return the median value of the data
    double median_value(void) const;

    // return the sample standard deviation of the data
    double stddev_value(void) const;

private:
    size_t len;
    std::pair<date, double> max_val;
    std::pair<date, double> min_val;
    double mean;
    double median;
    double stddev;

    double calculate_median(const std::map<date, double> &data) const;
    double calculate_stddev(const std::map<date, double> &data, double mean_val) const;
};