#pragma once

#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "play.hpp"

class Player
{
    std::map<unsigned int, Structured_line> lines;
    std::string character;
    std::thread mythread;
    std::ifstream input_file_stream;
    Play& play;

public:
    Player(Play& play_, std::string const name_, std::ifstream& input_file_stream_) :
        input_file_stream(std::move(input_file_stream_)), play(play_)
    {
        this->character = name_;
    }

    Player(Player&& right) :
           lines(std::move(right.lines)), mythread(std::move(right.mythread)),
           input_file_stream(std::move(right.input_file_stream)),  play(right.play)
    {
        this->character = right.character;
    }

    ~Player() {this->exit();}

    void read();
    void act();
    void enter();
    void exit();
};