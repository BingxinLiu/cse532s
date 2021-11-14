#include <algorithm>
#include <string>
#include "utilities.hpp"

using namespace std;

/**Learn from https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/ */

string ltrim(const string &s)
{
    size_t start_pos = s.find_first_not_of(WHITESPACE);
    return (start_pos == string::npos) ? "" : s.substr(start_pos);
}

string rtrim(const string &s)
{
    size_t end_pos = s.find_last_not_of(WHITESPACE);
    return (end_pos ==  string::npos) ? "" : s.substr(0, end_pos + 1);
}

string trim(const string &s)
{
    return rtrim(ltrim(s));
}

string get_path_to_directory(const string& path)
{
    size_t pos = path.find_last_of("\\/");
    string directory_path = (pos == string::npos) ? "" : string(path.substr(0, pos+1));
    return directory_path;
}