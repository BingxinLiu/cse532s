#pragma once

#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include "director.hpp"
#include "play.hpp"
#include "def.hpp"

using namespace std;

class Director;

class Player
{
    std::map<unsigned int, Structured_line> lines;
    std::thread work_thread;
    shared_ptr<Play> play;

    void start_working();
    bool is_leader();
    void assign_work_to_follower();
    void wait_for_this_scene_end();
    bool time_to_stop();
    void wait_for_recruited();


public:

    Director& director;

    std::string character;
    std::ifstream input_file_stream;
    unsigned int current_scene_index;
    bool activated = false;
    string input_file_name;

    mutex ready_to_read_mutex;
    condition_variable ready_to_read_cv;
    bool ready_to_read = false;

    // Update the Player class constructor, member variables, and methods so that they are compatible with the HS/HA or L/F design approach you take per the instructions above. In particular, you should consider whether the following changes are compatible with your approach (and if not should modify them accordingly). Again, please make sure to document your design decisions and rationale for this part as well, in your project report.

    // Update its constructor and member variables so that it only keeps track of a reference to the Play in which it is performing (and does not store the name of a character or a character part file stream).
    Player(shared_ptr<Play> play, Director& director);

    // Player(Player&& right) :
    //        lines(std::move(right.lines)), mythread(std::move(right.mythread)),
    //        input_file_stream(std::move(right.input_file_stream)),  play(right.play)
    // {
    //     this->character = right.character;
    // }

    ~Player() {this->exit();}

    void read();
    void act();
    void enter();
    void exit();
};