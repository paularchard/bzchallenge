#include <data_series.hpp>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

// Load data into the data_series. 
// In this case we're using boost::property_tree as a helper
// since it already implements a JSON parser for us
bool data_series::load(std::istream &data)
{
    pt::ptree proptree;

    try
    {
        pt::read_json(data, proptree);

        // validate and copy the keys and values into local map
        pt::ptree::assoc_iterator base = proptree.find("bpi");
        if (base != proptree.not_found())
        {
            for (auto it : base->second)
            {
                this->insert(std::make_pair(boost::gregorian::from_string(it.first), it.second.get_value<double>()));
            }
        }
        else
        {
            std::cerr << "\"bpi\" tag not found in data" << std::endl;
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