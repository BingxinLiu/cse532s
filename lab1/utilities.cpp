#include <algorithm>
#include "utilities.hpp"

/**Learn from https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/ */

std::string ltrim(const std::string &s)
{
    size_t start_pos = s.find_first_not_of(WHITESPACE);
    return (start_pos == std::string::npos) ? "" : s.substr(start_pos);
}

std::string rtrim(const std::string &s)
{
    size_t end_pos = s.find_last_not_of(WHITESPACE);
    return (end_pos ==  std::string::npos) ? "" : s.substr(0, end_pos + 1);
}

std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}