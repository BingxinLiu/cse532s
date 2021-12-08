#pragma once

#include <string>
#include <map>
#include <mutex>

typedef int director_id;
typedef std::string play_name;
typedef bool status;
typedef std::pair<director_id, status> director_status_pair;

class threadsafe_menu
{
    std::map<play_name, director_status_pair> items;
    std::mutex menu_mutex;

public:
    const std::string str();
};
