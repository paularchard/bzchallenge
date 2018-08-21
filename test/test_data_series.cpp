#include <gtest/gtest.h>
#include <data_series.hpp>

TEST(DataSeries, InitializeWithValidData)
{
    std::stringstream data;
    data << "{\"bpi\":{\"2018-07-17\":7314.9425}}";

    data_series series;
    ASSERT_TRUE(series.load(data));
    ASSERT_TRUE(series.size() == 1);
}

TEST(DataSeries, InitializeWithNoHeader)
{
    std::stringstream data;
    data << "{{\"2018-07-17\":7314.9425}}";

    data_series series;
    ASSERT_FALSE(series.load(data));
}

TEST(DataSeries, InitializeWithBadDate)
{
    std::stringstream data;
    data << "{{\"2018-13-17\":7314.9425}}";

    data_series series;
    ASSERT_FALSE(series.load(data));
}

TEST(DataSeries, InitializeWithBadValue)
{
    std::stringstream data;
    data << "{{\"2018-13-17\":\"xxx\"}}";

    data_series series;
    ASSERT_FALSE(series.load(data));
}

TEST(DataSeries, TestRetrieval)
{
    std::stringstream data;
    data << "{\"bpi\":{\"2018-07-17\":7314.9425}}";

    data_series series;
    ASSERT_TRUE(series.load(data));
    
    data_series::iterator it = series.begin();
    ASSERT_TRUE(it != series.end());
    ASSERT_EQ(it->first, date(2018, 7, 17));
    ASSERT_EQ(it->second, double(7314.9425));
}

TEST(DataSeries, TestIteration)
{
    std::stringstream data;
    data << "{\"bpi\":{\"2018-07-17\":7314.9425,\"2018-07-18\":7378.7575,\"2018-07-19\":7470.825,\"2018-07-20\":7330.5363}}";

    data_series series;
    ASSERT_TRUE(series.load(data));
    
    data_series::iterator it = series.begin();
    ASSERT_TRUE(it != series.end());
    it++;
    ASSERT_TRUE(it != series.end());
    it++;
    ASSERT_TRUE(it != series.end());
    it++;
    ASSERT_TRUE(it != series.end());
    it++;
    ASSERT_TRUE(it == series.end());
}

TEST(DataSeries, TestSortByDate)
{
    std::stringstream data;
    data << "{\"bpi\":{\"2018-07-20\":7314.9425,\"2018-07-19\":7378.7575,\"2018-07-18\":7470.825,\"2018-07-17\":7330.5363}}";

    data_series series;
    ASSERT_TRUE(series.load(data));
   
    data_series::iterator prev = series.begin();
    ASSERT_TRUE(prev != series.end());
    data_series::iterator next = prev;
    next++;
    while (next != series.end())
    {
        ASSERT_TRUE(next->first > prev->first);
        
        prev = next;
        next++;
    }
}
