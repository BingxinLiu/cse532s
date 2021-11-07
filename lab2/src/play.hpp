#pragma once

#include <vector>
#include <string>
#include <iterator>
#include <map>
#include <mutex>
#include <condition_variable>

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

class Play
{
    // takes and stores a reference to a container of strings (for the titles of the different scenes in the play)
    vector<string>& scenes_name;
    // initializes an iterator member variable to point to the beginning of that container
    vector<string>::iterator it = scenes_name.begin();
    // and if the container is non-empty prints out the string to which the iterator points to the standard output stream and then increments the iterator.


    // rename the counter member variable to something like line_counter
    unsigned int line_counter;

    // add a private numeric scene_fragment_counter member variable for the current scene fragment that is in progress, which the Play class constructor should initialize to 0 or 1 depending on how you handle increments and decrements to it in the enter and exit methods respectively (see below)
    unsigned int scene_fragment_counter;
    
    // add a private numeric on_stage member variable for the number of characters currently on stage in the current scene fragment, which the Play class constructor should initialize to 0;
    unsigned int on_stage_member_num;


    std::mutex recite_mutex;
    std::condition_variable recite_condv;

    std::string current_character;
    bool has_recite_first_character = false;

public:

    Play(const std::string play_name_) : play_name(play_name_), counter(1) 
    {
        this->current_character = "";
    }

    // update the signature of the public recite method so that in addition to an iterator over a container of structured lines, it also takes a numeric argument giving the number of the current scene fragment in which the lines are to be recited.
    void recite(std::map<unsigned int, Structured_line>::const_iterator& it, unsigned int current_scence);


    // add a public enter method that takes a numeric argument for the scene fragment that the character is attempting to enter: 
    void enter(unsigned int scence);

    // add a public exit method that: 
    void exit();
};

