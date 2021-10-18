#pragma once

#include <string>
#include <iterator>
#include <map>
#include <mutex>
#include <condition_variable>

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

class Play
{
    std::string play_name;
    // initialize to 0 or 1
    unsigned int counter;
    std::mutex recite_mutex;
    std::condition_variable recite_condv;

    std::string current_character;
    bool has_recite_first_character = false;

public:

    Play(const std::string play_name_) : play_name(play_name_), counter(1) 
    {
        this->current_character = "";
    }

    void recite(std::map<unsigned int, Structured_line>::const_iterator& it);
};

