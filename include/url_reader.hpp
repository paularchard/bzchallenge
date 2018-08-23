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

#pragma once

#include <string>

// This class reads a document from a provided URL and makes its content
// available via the response method. Currently only https:// URLs are 
// supported. Other schemes will cause an exception to be thrown.
//
class url_reader
{
public:
    // initialize the reader with a URL.
    // Throws exception on non-https scheme or empty hostname
    url_reader(const std::string &url);

    // retrieve the content of the URL. Returns true on success.
    bool read(void);

    // accessor for the response data
    const std::string &response(void) const { return data;}

private:
    std::string full_url;
    std::string data;
    std::string hostname;
    std::string path;
    std::string port;
};