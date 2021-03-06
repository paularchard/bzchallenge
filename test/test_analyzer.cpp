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

#include <gtest/gtest.h>
#include <data_series.hpp>
#include <map>
#include <common.hpp>
#include <analyzer.hpp>
#include <functional>

// helper function to verify that the called bound function 
// generates a domain_error exception
template <typename F>
void test_domain_exception(F func)
{
    try
    {
        func();
        FAIL() << "Expected exception to be thrown";
    }
    catch (std::exception &e)
    {
        std::cout << "caught exception " << e.what() << std::endl;
        EXPECT_EQ(e.what(), std::string("domain error"));
    }
}

TEST(Analyzer, TestEmptyData)
{
    std::map<date, double> data;
    analyzer a(data);

    EXPECT_EQ(a.count(), 0U);

    // all these functions should throw an exception when the data is empty
    test_domain_exception(std::bind(&analyzer::lowest, a));
    test_domain_exception(std::bind(&analyzer::highest, a));
    test_domain_exception(std::bind(&analyzer::mean_value, a));
    test_domain_exception(std::bind(&analyzer::median_value, a));
    test_domain_exception(std::bind(&analyzer::stddev_value, a));
}

// standard deviation requires at least two data elements, otherwise
// it will throw a domain_error exception
TEST(Analyzer, TestStdDevOnOneValue)
{
    std::map<date, double> data;
    data.insert(std::make_pair(date(2018, 1, 1), 1.));

    analyzer a(data);
    test_domain_exception(std::bind(&analyzer::stddev_value, a));
}

TEST(Analyzer, TestStdDevOnTwoValues)
{
    std::map<date, double> data;
    data.insert(std::make_pair(date(2018, 1, 1), 1.));
    data.insert(std::make_pair(date(2018, 1, 2), 2.));
    
    analyzer a(data);
    EXPECT_EQ(a.count(), 2U);

    try
    {
        double res = a.stddev_value();
        ASSERT_GT(res, 0.707);
        ASSERT_LT(res, 0.708);
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Analyzer, TestValidCase)
{
    std::map<date, double> data;
    data.insert(std::make_pair(date(2018, 1, 2), 3.));
    data.insert(std::make_pair(date(2018, 1, 1), 1.));
    data.insert(std::make_pair(date(2018, 1, 3), 2.));
    
    analyzer a(data);
    EXPECT_EQ(a.count(), 3U);

    try
    {
        EXPECT_EQ(a.mean_value(), 2.);
        EXPECT_EQ(a.median_value(), 2.);
        EXPECT_EQ(a.stddev_value(), 1.);
        EXPECT_EQ(a.highest(), std::make_pair(date(2018, 1, 2), 3.));
        EXPECT_EQ(a.lowest(), std::make_pair(date(2018, 1, 1), 1.));
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Analyzer, TestSplitMedian)
{
    std::map<date, double> data;
    data.insert(std::make_pair(date(2018, 1, 1), 1.));
    data.insert(std::make_pair(date(2018, 1, 2), 4.));
    data.insert(std::make_pair(date(2018, 1, 3), 3.));
    data.insert(std::make_pair(date(2018, 1, 4), 2.));
    
    analyzer a(data);
    try
    {
        ASSERT_EQ(a.median_value(), 2.5);
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Analyzer, TestDuplicateValues)
{
    std::map<date, double> data;
    data.insert(std::make_pair(date(2018, 1, 1), 1.));
    data.insert(std::make_pair(date(2018, 1, 2), 4.));
    data.insert(std::make_pair(date(2018, 1, 3), 1.));
    data.insert(std::make_pair(date(2018, 1, 4), 4.));
    
    analyzer a(data);
    try
    {
        // the most recent pairs with duplicate values 
        // should be returned in each case
        EXPECT_EQ(a.highest().first, date(2018, 1, 4));
        EXPECT_EQ(a.lowest().first, date(2018, 1, 3));
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}
