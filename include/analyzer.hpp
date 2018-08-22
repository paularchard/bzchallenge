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

    size_t count(void) const { return len; }
    
    std::pair<date, double> highest(void) const;
    std::pair<date, double> lowest(void) const;
    double mean_value(void) const;
    double median_value(void) const;
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