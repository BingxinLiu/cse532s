#pragma once

/**This file includes some common tools*/

#include <string>

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);
std::string get_path_to_directory(const string& path);