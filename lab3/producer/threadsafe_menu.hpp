#pragma once

#include <string>
#include <vector>
#include <list>
#include <mutex>

#include "../utilities/const.hpp"

typedef uint director_id;
typedef std::string play_name;
typedef bool status;

class threadsafe_menu
{
    std::list<std::pair<play_name, std::vector<director_id>>> avaliable_items;
    std::list<std::pair<play_name, std::vector<director_id>>> busy_items;
    std::mutex menu_mutex;

public:
    const std::string str();
    void reg_play(play_name play_name_, director_id id);
    void reg_busy_play(play_name play_name_, director_id id);
    uint pop_avaliable(play_name play_name_);
    play_name operator[](uint offset);
};
