#include <string>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <mutex>
#include "play.hpp"
#include "player.hpp"
#include "../utilities/utilities.hpp"

using namespace std;

Player::Player(shared_ptr<Play> play, director& director_) 
    : play(play) , 
    director_(director_)
{
    this->work_thread = thread([this](){
        start_working();
    });
}

void 
Player::start_working()
{
    // run until it's time to stop
    while ( !this->time_to_stop() )
    {
        this->wait_for_recruited();

        if ( this->activated )
        {
            // check if its self's turn to be a leader
            if ( this->is_leader() ) 
            {
                // assign work to follower
                this->assign_work_to_follower();
            }
            // after assigning work to each follower
            // turn self to be a follower
            this->enter();

        }

        
    }

}
// check if its time to go
bool
Player::time_to_stop()
{
    return this->play->finished;
}

// wait for wake up
void
Player::wait_for_recruited()
{
    unique_lock<mutex> lock(this->play->needed_player_num_mutex);
    this->play->needed_player_cv.wait(lock, [&](){
        return this->play->needed_player_num > 0
                || this->play->finished;
    });
    // if finished return imediately
    if ( this->play->finished ) 
    {
        lock.unlock();
        return;
    }
    // activate self and update needed number
    this->activated = true;
    this->play->needed_player_num--;
    lock.unlock();

}

// check if self could be a leader
bool
Player::is_leader()
{
    lock_guard<mutex> lock(this->play->leader_mutex);
    if ( this->play->has_leader ) return false;
    this->play->has_leader = true; 
    return true;
}

// as a leader assign work
void
Player::assign_work_to_follower()
{
    this->play->needed_player_cv.notify_all();
    // wait for enough player
    while ( this->play->needed_player_num != 0 ) 
    {
        this_thread::yield();
    }

    // Get the name of this scene
    unsigned int current_frag_index = distance((this->play->scenes_names).begin(), this->play->scene_it);

    // let director assign the character
    try
    {
        this->director_.cue(current_frag_index);
    } 
    catch(const std::exception& e)
    {
        cerr << e.what() << '\n';
    }

}

// Update the read method so that each time it is called it first empties the container of structured lines that is stored by the Player and then re-populates the container with lines from the character part file stream.
// a public member function read each line of its scripts and store as a structed line
void Player::read()
{

    unique_lock<mutex> lock(this->ready_to_read_mutex);
    if ( !this->ready_to_read )
        this->ready_to_read_cv.wait(lock, [&](){
            return this->ready_to_read;
        });
    lock.unlock();
    // clean old lines
    this->lines.clear();
    
    this->input_file_stream = ifstream(input_file_name);
    
    // check if the input file stream is valid
    if(!this->input_file_stream.good())
    {   
        lock_guard<mutex> lock (this->play->current_scene_end_mutex); 
        std::cerr << "Error: Input file stream broken. [ " << this->input_file_name << " ]" << std::endl;
        return;
    }

    // repeatedly (until it reaches the end of the input file stream referenced by its member variable)
    // reads a line from the input file stream;
    std::string line;
    while (getline(this->input_file_stream, line))
    {
        // clean whitespaces, skip empty line
        line = trim(line);
        if (line.empty()) continue;

        // extract order number and text
        unsigned int order_number;
        std::string text;
        std::istringstream issed_line(line);

        // converts the first whitespace delimited token of the line into a number (e.g., an unsigned int);
        if (!(issed_line >> order_number))
        {
            std::cerr << "Error: can not cope with line : " << line << std::endl;
            std::cerr << "\tcan not extract order number." << std::endl;
            continue;
        }

        // stores the rest of the line in a C++ style string as the text
        if (!getline(issed_line, text))
        {
            trim(text);
            if (!text.empty())
            {
                std::cerr << "Error: can not cope with line : " << line << std::endl;
                std::cerr << "\tcan not extract text." << std::endl;
                continue;
            } else
            {
                text = "";
            }
        }
        // It is possible that the method may see no well formed lines, which also is a reasonable case (e.g., to allow silent characters to enter and leave a scene in later labs) as long as it does recognize well formed lines if there are any. so we don't trim the line

        // if (and only if) both the number and some non-whitespace text were extracted from the line, inserts a structured line (based on the number, the character's name, and the text of the line following the number) into its container member variable (make sure that the container either preserves the order in which the lines appeared in the file, or reorders them by line number).
        
        this->lines.insert(pair<unsigned int, Structured_line>(order_number, Structured_line(this->character, text)));
    }
    // close the script file stream after it no longer in used.
    this->input_file_stream.close();
}

// Update the act method so that it is compatible with the HS/HA or L/F approach taken in your design. For example if you apply the HS/HA and Active Object patterns, the act method may repeatedly pull from the object's input queue and process each item it is given - alternatively, the act method could be the point where you initiate leader election if your design pursued the L/F variation.
void Player::act()
{
    //first initializes an iterator positioned at the beginning of the container of lines that the read method loaded from the file stream;
    //it should then repeatedly pass the iterator into a call to the recite method of the Play class referenced by the private member variable, until the iterator is past the last structured line in the container.
    std::map<unsigned int, Structured_line>::const_iterator it = this->lines.begin();
    
    while (it != this->lines.end())
    {
        // recite if it is this line's turn
        if (this->play->finished)
        {
            break;
        } 
        this->play->recite(std::ref(it), this->current_scene_index);
    }
    // clear self
    this->activated = false;
    this->input_file_name = "";
    this->character = "";
    this->ready_to_read = false;

}


//Update the enter and exit methods so that they are compatible with the appropriate thread pool management semantics for the HS/HA or L/F design approach you chose. In particular, issues of thread construction, thread spawning, and thread join or detach semantics should be considered for these methods.

//a public enter method that uses move semantics to launch a new thread and transfer ownership of it into the std::thread member variable. The thread should call the read method and then the act method of the object on which the enter method was called.
void
Player::enter()
{
    this->read();
    this->play->enter(this->current_scene_index);

    // wait for every player ready
    {
        unique_lock<mutex> lock(this->play->not_ready_players_num_mutex);
        this->play->not_ready_players_num--;
        if ( this->play->not_ready_players_num == 0 )
        {
            lock.unlock();
            this->play->not_ready_players_num_cv.notify_all();
        } else
        {
            this->play->not_ready_players_num_cv.wait(lock, [&](){
                return this->play->not_ready_players_num == 0;
            });
            lock.unlock();
        }
    }

    this->act();
    // try to exit and check status
    try
    {
        this->play->exit();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }  
     
}

void
Player::wait_for_this_scene_end()
{
    unique_lock<mutex> lock(this->play->current_scene_end_mutex);
    this->play->current_scene_end_cv.wait(lock, [&](){
        return this->play->current_scene_end;
    });
    lock.unlock();
}

// a public exit method that tests whether or not the std::thread member variable is joinable and if (and only if) it is calls its join method.
void
Player::exit()
{
    if (this->work_thread.joinable()) {
        this->work_thread.join();
    }
}