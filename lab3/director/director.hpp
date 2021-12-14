#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "ace/Event_Handler.h"

#include "connect_service.hpp"
#include "config.hpp"
#include "play.hpp"
#include "player.hpp"
#include "../utilities/const.hpp"

class Player;
class Play;


// A class on behalf of playing and communicate with producer
class director : public ACE_Event_Handler
{

    // director_id, which should be assigned from producer
    unsigned int director_id = 0;

    // socket wrapper used to communicate with producer
    ACE_SOCK_Stream* ace_sock_stream;

    // current scene's minimum players
    size_t min_players = 0;

    // vector for each scene name
    std::map<play_name, std::vector<scene_name>> scenes_names;
    // read each play's config file
    size_t parse_play_file(const std::string& play_config_file_name);
    // read each scene's config file
    size_t parse_script_file(const std::string& play_config_file_name, const std::string& script_config_file_name, size_t scene_config_index);
    // read the whole play's config file
    size_t parse_config_file(const std::string& scene_config_file_name);
    void recruit(size_t player_num);
    ~director();
public:

    // all of supported plays configuration
    std::map<play_name, Config_struct> configs;

    // keep tracking the current playing configuration
    Config_struct config;
    // keep tracking the play
    std::shared_ptr<Play> play;
    // keep tracking all of newed players
    std::list<shared_ptr<Player> > players;

    director(u_short port, std::string ip_address, int min_threads, std::vector<std::string> scripts_filename);

    // assign characters to each players according to fragment counter
    void cue(unsigned int frag_index);
    // start the first scene
    void start();

    // functions as a evnet handler
    virtual ACE_HANDLE get_handle() const;
    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
    virtual int handle_signal(int signal, siginfo_t* = 0, ucontext_t* = 0);

    // helper functions
    void regis_self();
    void send_play_list();
    void print_configs();
    void parse_receive_msg(std::string str);
    void send_msg(const std::string msg);
    unsigned int get_id() {return this->director_id;};

    // functions related to play
    void start_play(std::string playname, uint player_num);
    void stop_play();

    // functions for testing
    std::string config_to_str();
    std::string scenes_names_to_str(std::string playname);
};