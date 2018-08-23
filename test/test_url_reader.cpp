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
#include <url_reader.hpp>

TEST(UrlReader, TestEmptyUrl)
{
    url_reader reader("");
    ASSERT_EQ(reader.read(), false);
}

TEST(UrlReader, TestBadUrl)
{
    url_reader reader("htt://www.test.com/");
    ASSERT_EQ(reader.read(), false);
}

TEST(UrlReader, Test404)
{
    url_reader reader("http://www.cnn.com/garbage");
    ASSERT_EQ(reader.read(), false);
}

TEST(UrlReader, TestBadHost)
{
    url_reader reader("http://www.test2.com/");
    ASSERT_EQ(reader.read(), false);
}

TEST(UrlReader, TestValidUrl)
{
    url_reader reader("http://www.google.com/");
    ASSERT_EQ(reader.read(), true);
    const std::string &response = reader.response();

    // check that we have the start and end of the document
    ASSERT_NE(response.find("<!doctype html>"), std::string::npos);
    ASSERT_NE(response.find("</html>"), std::string::npos);
}
