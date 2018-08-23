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
#include <analyzer.hpp>
#include <report.hpp>
#include <fstream>

std::string 
get_report_template()
{
	std::string output;
	output = "\n" 
		"Price Report\n"
		"------------\n"
		"Records processed:  %count%\n"
		"Highest price:      %hi_price% on %hi_date%\n"
		"Lowest price:       %lo_price% on %lo_date%\n"
		"Average price:      %mean%\n"
		"Median price:       %median%\n"
		"Standard deviation: %stddev%\n\n";

	return output;
}

int
main(int argc, char *argv[])
{
	// there should only be a single command-line argument, the filename
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
		exit(-1);
	}

	std::fstream file(argv[1], std::fstream::in);
	if (file.good())
	{
		data_series data;
		if (data.load(file))
		{
			try
			{
				analyzer a(data);
				std::map<std::string, std::string> tokens;
				tokens.insert(std::make_pair("count", std::to_string(a.count())));
				tokens.insert(std::make_pair("hi_date", boost::gregorian::to_iso_extended_string(a.highest().first)));
				tokens.insert(std::make_pair("hi_price", std::to_string(a.highest().second)));
				tokens.insert(std::make_pair("lo_date", boost::gregorian::to_iso_extended_string(a.lowest().first)));
				tokens.insert(std::make_pair("lo_price", std::to_string(a.lowest().second)));
				tokens.insert(std::make_pair("mean", std::to_string(a.mean_value())));
				tokens.insert(std::make_pair("median", std::to_string(a.median_value())));
				tokens.insert(std::make_pair("stddev", std::to_string(a.stddev_value())));

				std::stringstream report_template(get_report_template());
				std::string output = report::make_report(report_template, tokens);
				std::cout << output;
			}
			catch (std::exception &e)
			{
				std::cerr << "An error occurred during processing: " << e.what() << std::endl;
			}
		}
		else
		{
			std::cerr << "There was an error loading data from " << std::string(argv[1]) << std::endl;
		}

		file.close();
	}
	else
	{
			std::cerr << "Unable to open file " << std::string(argv[1]) << std::endl;
	}

	return 0;
}
