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

#include <data_series.hpp>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

// tag name to look for in JSON data
static const std::string BASE_KEY = "bpi";

// Load data into the data_series. 
// In this case we're using boost::property_tree as a helper
// since it already implements a JSON parser for us
bool 
data_series::load(std::istream &strm)
{
    // clear any existing data
    this->data.clear();

    pt::ptree proptree;
    try
    {
        pt::read_json(strm, proptree);

        // validate and copy the keys and values into local map
        pt::ptree::assoc_iterator base = proptree.find(BASE_KEY);
        if (base != proptree.not_found())
        {
            for (auto it : base->second)
            {
                this->data.insert(std::make_pair(boost::gregorian::from_string(it.first), it.second.get_value<double>()));
            }
        }
        else
        {
            std::cerr << "\"" << BASE_KEY << "\" key name not found in data" << std::endl;
            return false;
        }
    }
    catch (pt::json_parser::json_parser_error &e)
    {
        std::cerr << "JSON format error" << std::endl;
        return false;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error during parsing of data: " << e.what() << std::endl;
        return false;
    }

    return true;
}