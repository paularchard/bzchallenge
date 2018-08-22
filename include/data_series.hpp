#pragma once

#include <map>
#include <iostream>
#include <common.hpp>

// This class represents a date-based data series which may accessed like a standard map.
// The input is taken from a custom, JSON-based data structure (as a stream) of the format:
//   {"bpi":{"<date>":<price>,...}...}
// Only date/price values under the "bpi" key are considered. All other data are ignored.
//
// The map key is a boost gregorian date and the map value is a double representing a price
// the series is defined to be sorted by (unique) date
class data_series : public std::map<date, double>
{
public:
    bool load(std::istream &data);
};
