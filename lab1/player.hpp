#pragma once

#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "play.hpp"

class Player
{
    // Please make sure that whatever container type you use has a suitable iterator type that can be positioned at the first element of the container, incremented, and tested for having gone past the last element of the container.
    std::map<unsigned int, Structured_line> lines;
    std::string character;
    std::thread mythread;
    std::ifstream input_file_stream;
    Play& play;

public:
    Player(Play& play_, std::string const name_, std::ifstream& input_file_stream_) :
        input_file_stream(std::move(input_file_stream_)), play(play_)
    {
        // The Player class constructor also should default construct a private std::thread member variable.
        this->character = name_;
        // debug
        // std::cout << "Player " << name_ << " initialized!" << std::endl;
    }

    Player(Player&& right) :
           lines(std::move(right.lines)), mythread(std::move(right.mythread)),
           input_file_stream(std::move(right.input_file_stream)),  play(right.play)
    {
        this->character = right.character;
        // std::unique_lock<std::mutex> lock(cout_mutex);
        // std::cout << this->lines.character << std::endl;
        // std::cout << (this->input_file_stream.good() ? "good" : "not good") << std::endl;
    }

    void read();
    void act();
    void enter();
    void exit();
};