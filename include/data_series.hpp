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
#include <iostream>
#include <common.hpp>

// This class implements a date-based data series which may accessed as a standard map.
// The input is taken from a custom, JSON-based data structure (as a stream) of the format:
//   {"bpi":{"<date>":<price>,...}...}
// Only date/price values under the "bpi" key are considered. All other data are ignored.
//
// The map key is a boost gregorian date and the map value is a double representing a price
// the series is defined to be sorted by (unique) date
class data_series
{
public:
    bool load(std::istream &strm);
    const std::map<date, double> &get() const { return data; }

private:
    std::map<date, double> data;
};
