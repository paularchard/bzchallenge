#pragma once

#include <map>
#include <common.hpp>
#include <stdexcept>

class analyzer
{
public:
    analyzer(const std::map<date, double> &data) {}

    size_t count(void) const { return 0; }
    std::pair<date, double> highest(void) const { return std::make_pair(date(), 0.); }
    std::pair<date, double> lowest(void) const { return std::make_pair(date(), 0.); }
    double mean_value(void) const { return 0.; }
    double median_value(void) const { return 0.; }
    double stddev_value(void) const { return 0.; }
};