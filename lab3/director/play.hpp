#pragma once

#include <vector>
#include <string>
#include <iterator>
#include <map>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "director.hpp"
#include "config.hpp"

using namespace std;

struct Structured_line
{
    std::string character;
    std::string text;
    Structured_line(std::string character_, std::string text_) :
        character(character_), text(text_)
    {}
    Structured_line(Structured_line&& right) :
        character(std::move(right.character)), text(std::move(right.text))
    {}
};

class director;

class Play
{
    

    // rename the counter member variable to something like line_counter
    mutex line_counter_mutex;
    unsigned int line_counter = 0;

    // add a private numeric scene_fragment_counter member variable for the current scene fragment that is in progress, which the Play class constructor should initialize to 0 or 1 depending on how you handle increments and decrements to it in the enter and exit methods respectively (see below)
    mutex scene_fragment_counter_mutex;
    unsigned int scene_fragment_counter = 0;

    condition_variable start_scene_cv;
    
    // add a private numeric on_stage member variable for the number of characters currently on stage in the current scene fragment, which the Play class constructor should initialize to 0;
    mutex on_stage_member_num_mutex;
    unsigned int on_stage_member_num = 0;


    // a flag indicating the current scene has finished
    mutex current_scene_finished_mutex;
    bool current_scene_finished;

    // recite mutex to keep serialize
    std::mutex recite_mutex;
    std::condition_variable recite_condv;

    // keep tracking current character
    std::string current_character;
    bool has_recite_first_character = false;

public:
    // a flag indicating every player ready
    mutex not_ready_players_num_mutex;
    condition_variable not_ready_players_num_cv;
    unsigned int not_ready_players_num = 0;

    // a flag indicating the current scene has finished  
    bool current_scene_end = false;
    mutex current_scene_end_mutex;
    condition_variable current_scene_end_cv;

    // a flag indicating the play has finished
    bool finished = false;

    // a flag indicating if there is a leader
    mutex leader_mutex;
    bool has_leader = false;

    // number of needed player in this play
    mutex needed_player_num_mutex;
    condition_variable needed_player_cv;
    unsigned int needed_player_num = 0;

    // takes and stores a reference to a container of strings (for the titles of the different scenes in the play)
    const vector<string>& scenes_names;

    // initializes an iterator member variable to point to the beginning of that container
    vector<string>::const_iterator scene_it;

    const Config_struct& config;

    // director
    director& director_;
    const std::string playname;

    explicit Play(const Config_struct& config, const vector<string>& scenes_names, director& director_, const std::string playname);


    // update the signature of the public recite method so that in addition to an iterator over a container of structured lines, it also takes a numeric argument giving the number of the current scene fragment in which the lines are to be recited.
    void recite(std::map<unsigned int, Structured_line>::const_iterator& it, unsigned int current_scene);


    // add a public enter method that takes a numeric argument for the scene fragment that the character is attempting to enter: 
    void enter(unsigned int scene_index);

    // add a public exit method that: 
    void exit();
};

