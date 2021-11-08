#include <iostream>
#include <map>
#include <algorithm>
#include <mutex>
#include "play.hpp"

Play::Play(const Config_struct& config, const vector<string>& scenes_names) :
    config(config),
    scenes_names(scenes_names),
    line_counter(0),
    scene_fragment_counter(0),
    on_stage_member_num(0)
{
    this->scene_it = scenes_names.cbegin();
    this->current_character = "";

    // initialize counter
    
}

// Also update the recite method's implementation so that it compares the values of both the line_counter and scene_fragment_counter member variables, to the line number of the structured line to which the passed iterator refers and the passed scene fragment number, respectively.
// a function play the play
void
Play::recite(std::map<unsigned int, Structured_line>::const_iterator& it, unsigned int current_scene)
{
    unique_lock<mutex> lock(this->recite_mutex);

    // While the scene_fragment_counter member variable is less than the passed scene fragment number, or is equal but the line_counter member variable is less than the line number in the structured line referenced by the passed iterator, the recite method should repeatedly wait on a condition variable, until the line_counter and scene_fragment_counter member variables reach the values given in the the corresponding passed data.
    if ( this->scene_fragment_counter <= current_scene )
    {
        if ( this->scene_fragment_counter < current_scene
             || this->line_counter < (*it).first )
        {
            this->recite_condv.wait(lock, [&](){
                return (this->scene_fragment_counter == current_scene) && (this->line_counter == (*it).first);
            });
        }
        if (this->scene_fragment_counter == current_scene
             && this->line_counter == (*it).first )
        {

            // When the scene_fragment_counter member variable equals the passed scene fragment number and the line_counter member variable equals the structured line's number, the recite method should print out the line (to cout, the standard output stream), increment the iterator, notify all other threads waiting on the condition variable, and return.

            /**
             * keep track of which character is currently speaking,
             * and when a change occurs should print out (1) a blank line and (2) a line with 
             * the name of the new character (with a period at the end) before printing out 
             * the new character's subsequent line(s).
             */
            if (current_character != (*it).second.character)
            {
                if (!this->has_recite_first_character)
                {
                    std::cout << (*it).second.character << "." << std::endl;
                    this->has_recite_first_character = true;
                } else
                    std::cout << "\n" << (*it).second.character << ". " << std::endl;
                current_character = (*it).second.character;
            }

            // print out the line
            std::cout << (*it).second.text << std::endl;
            // increment the iterator
            it++;

            // increment the counter once per line that is delivered
            this->line_counter++;
            
            // notify all other threads waiting on the condition variable
            lock.unlock();
            this->recite_condv.notify_all();

            // return
            return;
        
        }
    } 

    // To avoid potential deadlocks and other problems if a badly formed script fragment is provided or somehow the program loses track of the count, if the scene_fragment_counter member variable is ever greater than the passed scene fragment number, or is equal but the line_counter member variable is greater than the line number in the structured line referenced by the passed iterator, the recite method should 
    
    // (1) write a line containing an error message to the standard error stream, 
    std::cerr << "WARNING: couter is larger than lines_number" << std::endl;

    // (2) increment the iterator (but not the line counter), 
    it++;

    // (3) notify all other threads waiting on the condition variable, and 
    lock.unlock();
    this->recite_condv.notify_all();
    

    //(4) return.
    return;

    // we don't need to deal with the situation of smaller since we will wait until it is equal or larger than counter
}


void 
Play::enter(unsigned int scene_index) 
{
    unique_lock<mutex> lock(this->scene_fragment_counter_mutex);
    // if the passed value is less than the scene_fragment_counter member variable, the method should simply fail (by returning a non-zero error code, or throwing an exception, etc.); 
    if ( scene_index < this->scene_fragment_counter )
    {
        lock.unlock();
        cerr << "ERROR: trying to enter a finished scene" << endl;
        // TODO: throw something
        throw invalid_argument("?");
    }

    // if the passed value is equal to the scene_fragment_counter member variable, the method should increment the on_stage member variable and return; 
    if ( scene_index == this->scene_fragment_counter )
    {
        lock.unlock();
        on_stage_member_num++;
        return;
    }
    
    // if the passed value is greater than the scene_fragment_counter member variable, the method should wait on a condition variable until they are equal, and then increment the on_stage member variable and return.
    if ( scene_index > this->scene_fragment_counter )
    {
        this->start_scene_cv.wait(lock, [&](){
            return scene_index == this->scene_fragment_counter;
        });
        lock.unlock();
        on_stage_member_num++;
        return;
    }

}


void 
Play::exit()
{
    lock_guard<mutex> lock(this->on_stage_member_num_mutex);

    //(1) if the on_stage member variable is greater than 1 simply decrements the on_stage member variable; 
    if ( this->on_stage_member_num > 1) 
    {
        this->on_stage_member_num--;
        return;
    }

    // or (2) if the on_stage member variable is less than 1 throws an exception or returns a non-zero error code; 
    if ( this->on_stage_member_num < 1 )
        throw invalid_argument("Error: There isn't any player on stage.");

    // or (3) if the on_stage member variable is exactly 1 and (d) signals the condition variable used by the enter method.
    if ( this->on_stage_member_num == 1) 
    {
        // then (a) decrements the on_stage member variable, 
        this->on_stage_member_num--;
        // (b) increments the scene_fragment_counter member variable, 
        this->scene_fragment_counter++;
        // (c) if the iterator member variable is not already past the end of the container of scene titles prints out the string the iterator member variable currently references if that string is non-empty and then (whether or not the string was empty) increments the iterator member variable, 
        if ( this->scene_it != this->scenes_names.end() )
        {
            if ( !(*this->scene_it).empty() )
                cout << *scene_it << endl;
            this->scene_it++;

            // set new scene
            {
                lock_guard<mutex> lock(this->needed_player_num_mutex);
                this->needed_player_num = this->config[this->scene_fragment_counter].second.size();
            }
            {
                lock_guard<mutex> lock(this->leader_mutex);
                this->has_leader = false;
            }
            this->needed_player_cv.notify_one();

        } else {
            // if finished, set finish flag
            this->finished = true;
        }

    }
}