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

#include <analyzer.hpp>
#include <cmath>
#include <numeric>

analyzer::analyzer(const std::map<date, double> &data)
: len(0U)
{
    double sum = 0.;
    for (auto &d : data)
    {
        if (!this->len || (d.second >= this->max_val.second))
            this->max_val = d;

        if (!this->len || (d.second <= this->min_val.second))
            this->min_val = d;

        this->len++;
        sum += d.second;
    }

    if (this->len)
    {
        this->mean = sum / this->len;
        this->median = this->calculate_median(data);
    }

    if (this->len > 1)
        this->stddev = this->calculate_stddev(data, this->mean);
}

std::pair<date, double> 
analyzer::highest(void) const
{
    if (!(this->count()))
        throw std::domain_error("domain error");

    return this->max_val;
}

std::pair<date, double>
analyzer::lowest(void) const
{
    if (!(this->count()))
        throw std::domain_error("domain error");

    return this->min_val;
}

double
analyzer::mean_value(void) const
{
    if (!(this->count()))
        throw std::domain_error("domain error");

    return this->mean;
}

double
analyzer::median_value(void) const
{
    if (!(this->count()))
        throw std::domain_error("domain error");

    return this->median;
}

double
analyzer::stddev_value(void) const
{
    if (this->count() < 2)
        throw std::domain_error("domain error");

    return this->stddev;
}

double
analyzer::calculate_median(const std::map<date, double> &data) const
{
    // to calculate the median we need to sort the data by value
    std::vector<double> values;
    values.reserve(this->count());
    std::for_each (data.begin(), data.end(), 
        [&values](auto &val) 
        {
            values.push_back(val.second);
        });
    std::sort(values.begin(), values.end());

    auto size = values.size();
    assert(size);

    if (size % 2)
    {
        return values[size / 2];
    }
    else
    {
        return (values[(size / 2) - 1] + values[size / 2]) / 2;
    }
}

double
analyzer::calculate_stddev(const std::map<date, double> &data, double mean_val) const
{
    assert(this->count() >= 2);
    
    auto variance = std::accumulate(data.begin(), data.end(), 0.,
        [mean_val](auto &val1, auto &val2)
        {
            return val1 + ((val2.second - mean_val) * (val2.second - mean_val));
        });
    variance /= (this->count() - 1);
    
    return std::sqrt(variance);
}