#pragma once

#include <map>
#include <common.hpp>
#include <stdexcept>

class analyzer
{
public:
    analyzer(const std::map<date, double> &_data) : data(_data) {}

    size_t count(void) const { return data.size(); }
    std::pair<date, double> highest(void) const;
    std::pair<date, double> lowest(void) const;
    double mean_value(void) const;
    double median_value(void) const;
    double stddev_value(void) const;

private:
    const std::map<date, double> &data;
};