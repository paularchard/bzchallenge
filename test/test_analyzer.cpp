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

    ASSERT_EQ(a.count(), 0);

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
    ASSERT_EQ(a.count(), 2);

    try
    {
        double res = a.stddev_value();
        ASSERT_GT(res, 1.707);
        ASSERT_LT(res, 1.708);
    }
    catch (std::exception &e)
    {
        FAIL() << "Unexpected exception thrown";
    }
}

TEST(Analyzer, TestValidCase)
{
    std::map<date, double> data;
    data.insert(std::make_pair(date(2018, 1, 1), 1.));
    data.insert(std::make_pair(date(2018, 1, 2), 2.));
    data.insert(std::make_pair(date(2018, 1, 3), 3.));
    
    analyzer a(data);
    ASSERT_EQ(a.count(), 3);

    try
    {
        ASSERT_EQ(a.mean_value(), 2.);
        ASSERT_EQ(a.median_value(), 2.);
        ASSERT_EQ(a.stddev_value(), 1.);
        ASSERT_EQ(a.highest(), std::make_pair(date(2018, 1, 3), 3.));
        ASSERT_EQ(a.lowest(), std::make_pair(date(2018, 1, 1), 1.));
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
    data.insert(std::make_pair(date(2018, 1, 2), 2.));
    data.insert(std::make_pair(date(2018, 1, 3), 3.));
    data.insert(std::make_pair(date(2018, 1, 4), 4.));
    
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

