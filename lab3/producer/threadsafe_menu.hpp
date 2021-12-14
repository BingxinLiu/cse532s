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

// a class on behalf of recording directors supported play
// provide the service of register/delete/refresh the menu
class threadsafe_menu
{
    // available map for recording the number of available directors for a play.
    std::map<play_name, size_t> play_name_available_map;
    // the recording for each director and its supported plays.
    std::map<director_id, std::pair<status, std::set<play_name>>> menu;
    std::mutex menu_mutex;

public:
    // 'stringfy' the menu
    const std::string str();
    // register a
    void reg_play(play_name play_name_, director_id id);
    // pop an available director's id
    uint pop_avaliable(play_name play_name_);
    // pop a busy director's id
    uint pop_busy_play(play_name play_name_);
    // get the playname via index
    play_name operator[](uint offset);
    // remove a director's record
    void clean_with_id(uint id);
    // set a director as idle
    void set_idle_with_id(uint id);
    // set a director as busy
    void set_busy_with_id(uint id);
    // check whether empty, return # of records
    size_t is_empty();
};
