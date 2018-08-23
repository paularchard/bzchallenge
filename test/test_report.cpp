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
#include <report.hpp>
#include <sstream>

using namespace report;

TEST(Report, TestEmptyData)
{
    try
    {
        std::stringstream ss;
        std::string report = make_report(ss, std::map<std::string, std::string>());
        ASSERT_EQ(report, std::string());
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Report, TestValidData)
{
    try
    {
        std::stringstream ss;
        ss << "This is a %val% of make_report";

        std::map<std::string, std::string> tokens;
        tokens.insert(std::make_pair("val", "test"));

        std::string report = make_report(ss, tokens);
        ASSERT_EQ(report, std::string("This is a test of make_report"));
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Report, TestTokenWithNoSpace)
{
    try
    {
        std::stringstream ss;
        ss << "This is a test of %val%_report";

        std::map<std::string, std::string> tokens;
        tokens.insert(std::make_pair("val", "make"));

        std::string report = make_report(ss, tokens);
        ASSERT_EQ(report, std::string("This is a test of make_report"));
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Report, TestEscapedDelimiter)
{
    try
    {
        std::stringstream ss;
        ss << "I %val% this book at 15%% off";

        std::map<std::string, std::string> tokens;
        tokens.insert(std::make_pair("val", "purchased"));

        std::string report = make_report(ss, tokens);
        ASSERT_EQ(report, std::string("I purchased this book at 15% off"));
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Report, TestMultipleTokens)
{
    try
    {
        std::stringstream ss;
        ss << "I %action% this %item% at Barnes & Noble";

        std::map<std::string, std::string> tokens;
        tokens.insert(std::make_pair("action", "purchased"));
        tokens.insert(std::make_pair("item", "book"));

        std::string report = make_report(ss, tokens);
        ASSERT_EQ(report, std::string("I purchased this book at Barnes & Noble"));
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Report, TestMissingTokenValue)
{
    try
    {
        std::stringstream ss;
        ss << "I %action% this %item% at Barnes & Noble";

        std::map<std::string, std::string> tokens;
        tokens.insert(std::make_pair("action", "purchased"));

        std::string report = make_report(ss, tokens);
        ASSERT_EQ(report, std::string("I purchased this  at Barnes & Noble"));
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Report, TestUnterminatedToken)
{
    try
    {
        std::stringstream ss;
        ss << "This is a test of %val";

        std::map<std::string, std::string> tokens;
        tokens.insert(std::make_pair("val", "make_report"));

        std::string report = make_report(ss, tokens);
        FAIL() << "Expected exception to be thrown";
    }
    catch (std::exception &e)
    {
        std::cout << "caught exception " << e.what() << std::endl;
        EXPECT_EQ(e.what(), std::string("invalid argument"));
    }
}

TEST(Report, TestTokenWithSpace)
{
    try
    {
        std::stringstream ss;
        ss << "This is a %token name% of make_report";

        std::map<std::string, std::string> tokens;
        tokens.insert(std::make_pair("token name", "test"));

        std::string report = make_report(ss, tokens);
        FAIL() << "Expected exception to be thrown";
    }
    catch (std::exception &e)
    {
        std::cout << "caught exception " << e.what() << std::endl;
        EXPECT_EQ(e.what(), std::string("invalid argument"));
    }
}
