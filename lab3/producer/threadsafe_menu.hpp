#pragma once

#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <map>
#include <set>
#include <algorithm>

#include "../utilities/const.hpp"

enum status {AVAILABLE, UNCLEAR, UNAVAILABLE};

typedef uint director_id;
typedef std::string play_name;
typedef std::map<director_id, std::pair<status, std::set<play_name>>> menu_type;
typedef std::pair<status, std::set<play_name>> status_play_names_type;
typedef std::set<play_name> play_names_type;

class threadsafe_menu
{
    std::map<play_name, size_t> play_name_available_map;
    std::map<director_id, std::pair<status, std::set<play_name>>> menu;
    std::mutex menu_mutex;

public:
    const std::string str();
    //const std::string test_str();
    void reg_play(play_name play_name_, director_id id);
    //void reg_busy_play(play_name play_name_, director_id id);
    uint pop_avaliable(play_name play_name_);
    uint pop_busy_play(play_name play_name_);
    play_name operator[](uint offset);
    void clean_with_id(uint id);
    void set_idle_with_id(uint id);
    void set_busy_with_id(uint id);
    //bool find_in_busy(director_id id);
    void empty();
};
