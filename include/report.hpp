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

#include <iostream>
#include <string>
#include <map>

namespace report
{

// Build a report by merging the token/value pairs in vars into the template 
// read from tpl. The tokens are encoded in the template using %token-name% and
// must not contain whitespace. Literal % symbols must be escaped using %%.  
// Unterminated tokens with cause a std::invalid_argument exception to be
// thrown. Unmatched tokens will be replaced with an empty string.
//
std::string make_report(std::istream &tpl, const std::map<std::string,
    std::string> &vars);

} // namespace report
