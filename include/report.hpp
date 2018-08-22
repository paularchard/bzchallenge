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
