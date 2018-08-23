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

#include <common.hpp>
#include <data_series.hpp>
#include <analyzer.hpp>
#include <report.hpp>
#include <url_reader.hpp>
#include <fstream>
#include <memory>

static const std::string BASE_URL = "https://api.coindesk.com/v1/bpi/historical/close.json";

std::string 
get_report_template(void)
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

void
display_usage(const char *prog_name)
{
	std::cout << "\nUsage: " << prog_name << " filename" << std::endl;
	std::cout << "Usage: " << prog_name << " start-date end-date" << std::endl << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  filename      JSON file containing price data" << std::endl;
	std::cout << "  start-date    First date for price info (yyyy-mm-dd)" << std::endl;
	std::cout << "  end-date      Last date for price info (yyyy-mm-dd)" << std::endl << std::endl;

}

std::unique_ptr<std::istream>
get_input_stream_from_args(int argc, char *argv[])
{
	if (argc > 1 && std::string(argv[1]) == "--help")
	{
		display_usage(argv[0]);
		exit(0);
	}

	if (argc == 2)
	{
		// if there's a single argument, we assume it's a filename
		auto file = std::make_unique<std::fstream>(argv[1], std::fstream::in);
		if (!(file->good()))
		{
			std::cerr << "Unable to open file " << std::string(argv[1]) << std::endl;
			exit(-1);
		}

		// note - fstream will close file when it goes out of scope
		return file;
	}
	else if (argc == 3)
	{
		// if there's two arguments, try to turn them into dates
		try
		{
			auto start = boost::gregorian::to_iso_extended_string(
				boost::gregorian::from_string(std::string(argv[1])));
			auto end = boost::gregorian::to_iso_extended_string(
				boost::gregorian::from_string(std::string(argv[2])));

			auto url = BASE_URL + "?start=" + start + "&end=" + end;
			auto reader = url_reader(url);
			if (reader.read())
			{
				auto strm = std::make_unique<std::stringstream>();
				strm->str(reader.response());
				return strm;
			}
		}
		catch (const boost::bad_lexical_cast &e)
		{
			std::cerr << "\nError: the dates you provided were not valid." << std::endl;
			std::cerr << "Please use dates in the form yyyy-mm-dd, e.g. 2018-01-31." << std::endl;
			
			display_usage(argv[0]);
			exit(-1);
		}
		catch (const std::exception &e)
		{
			std::cerr << "An exception occurred reading data: " << e.what() << std::endl;
		}

		exit(-1);
	}
	else
	{
		display_usage(argv[0]);
		exit(-1);
	}
}

int
main(int argc, char *argv[])
{
	auto strm = get_input_stream_from_args(argc, argv);
	data_series data;
	if (data.load(*strm))
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
			auto output = report::make_report(report_template, tokens);
			std::cout << output;
		}
		catch (std::exception &e)
		{
			std::cerr << "An error occurred during processing: " << e.what() << std::endl;
		}
	}
	else
	{
		std::cerr << "An error occurred loading data" << std::endl;
	}

	return 0;
}
