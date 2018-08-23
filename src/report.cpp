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

#include <report.hpp>

namespace report
{

static const int DELIMITER = '%';

static std::string
extract_token(std::istream &strm)
{
    std::string token;

    char ch;
    while (strm.get(ch))
    {
        if (std::isspace(ch))
            throw std::invalid_argument("invalid argument");

        if (ch == DELIMITER)
        {
            // found the end of the token
            return token;
        }

        token += ch;
    }

    // we reached the end of the stream without finding the end
    // delimiter of the token
    throw std::invalid_argument("invalid argument");    
}

static std::string
substitute_token(const std::string &token, const std::map<std::string, 
    std::string> &vars)
{
    std::string value;

    const std::map<std::string, std::string>::const_iterator it = vars.find(token);
    if (it != vars.end())
        value = it->second;

    return value;
}

std::string 
make_report(std::istream &tpl, const std::map<std::string, std::string> &vars)
{
    std::string output;

    char ch;
    while (tpl.get(ch))
    {
        if (ch == DELIMITER)
        {
            if (tpl.peek() == DELIMITER)
            {
                // literal %
                // consume next character and fall through to output it
                tpl.get(ch);
            }
            else
            {
                // we have the start of a token
                std::string token = extract_token(tpl);
                output += substitute_token(token, vars);
                continue;
            }
        }

        // append the current character to the output
        output += ch;
    }

    return output;
}

} // namespace report