#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <list>

#include "play.hpp"
#include "player.hpp"
#include "def.hpp"


using namespace std;

class Player;

class Director
{
    // vector for each scene name
    vector<scene_name> scenes_names;
    // read each scene's config file
    size_t parse_script_file(const string& script_config_file_name, size_t scene_config_index);
    // read the whole play's config file
    size_t parse_config_file(const string& scene_config_file_name);
    void recruit(size_t player_num);

public:
    // keep tracking the configuration
    Config_struct config;
    // keep tracking the play
    shared_ptr<Play> play;
    // keep tracking all of newed players
    list<shared_ptr<Player> > players;
    //constructor
    Director(const string& script_file_name, unsigned int min_player_number);
    ~Director();

    // assign characters to each players according to fragment counter
    void cue(unsigned int frag_index);
    // start the first scene
    void start();
};