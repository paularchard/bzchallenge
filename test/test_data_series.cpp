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

TEST(DataSeries, InitializeWithValidData)
{
    std::stringstream strm;
    strm << "{\"bpi\":{\"2018-07-17\":7314.9425}}";

    data_series series;
    ASSERT_TRUE(series.load(strm));
    ASSERT_TRUE(series.get().size() == 1);
}

TEST(DataSeries, InitializeWithInvalidJson)
{
    std::stringstream strm;
    strm << "{{\"2018-07-17\":7314.9425}";

    data_series series;
    ASSERT_FALSE(series.load(strm));
}

TEST(DataSeries, InitializeWithNoHeader)
{
    std::stringstream strm;
    strm << "{\"2018-07-17\":7314.9425}";

    data_series series;
    ASSERT_FALSE(series.load(strm));
}

TEST(DataSeries, InitializeWithBadDate)
{
    std::stringstream strm;
    strm << "{\"bpi\":{\"2018-13-17\":7314.9425}}";

    data_series series;
    ASSERT_FALSE(series.load(strm));
}

TEST(DataSeries, InitializeWithBadValue)
{
    std::stringstream strm;
    strm << "{\"bpi\":{\"2018-12-17\":\"xxx\"}}";

    data_series series;
    ASSERT_FALSE(series.load(strm));
}

TEST(DataSeries, InitializeTwice)
{
    std::stringstream strm;
    strm << "{\"bpi\":{\"2018-07-17\":7314.9425}}";

    data_series series;
    ASSERT_TRUE(series.load(strm));
    ASSERT_TRUE(series.get().size() == 1);

    // calling load again should overwrite any existing data, not add to it
    std::stringstream strm2;
    strm2 << "{\"bpi\":{\"2018-07-18\":7314.9425}}";
    ASSERT_TRUE(series.load(strm2));
    ASSERT_TRUE(series.get().size() == 1);
}

TEST(DataSeries, TestRetrieval)
{
    std::stringstream strm;
    strm << "{\"bpi\":{\"2018-07-17\":7314.9425}}";

    data_series series;
    ASSERT_TRUE(series.load(strm));
    
    auto &data = series.get();
    auto it = data.begin();
    ASSERT_TRUE(it != data.end());
    ASSERT_EQ(it->first, date(2018, 7, 17));
    ASSERT_EQ(it->second, double(7314.9425));
}

TEST(DataSeries, TestIteration)
{
    std::stringstream strm;
    strm << "{\"bpi\":{\"2018-07-17\":7314.9425,\"2018-07-18\":7378.7575,\"2018-07-19\":7470.825,\"2018-07-20\":7330.5363}}";

    data_series series;
    ASSERT_TRUE(series.load(strm));
    
    auto &data = series.get();
    auto it = data.begin();
    ASSERT_TRUE(it != data.end());
    it++;
    ASSERT_TRUE(it != data.end());
    it++;
    ASSERT_TRUE(it != data.end());
    it++;
    ASSERT_TRUE(it != data.end());
    it++;
    ASSERT_TRUE(it == data.end());
}

TEST(DataSeries, TestSortByDate)
{
    std::stringstream strm;
    strm << "{\"bpi\":{\"2018-07-20\":7314.9425,\"2018-07-19\":7378.7575,\"2018-07-18\":7470.825,\"2018-07-17\":7330.5363}}";

    data_series series;
    ASSERT_TRUE(series.load(strm));

    auto &data = series.get();
    auto prev = data.begin();
    ASSERT_TRUE(prev != data.end());

    auto next = prev;
    next++;
    while (next != data.end())
    {
        ASSERT_TRUE(next->first > prev->first);
        
        prev = next;
        next++;
    }
}
