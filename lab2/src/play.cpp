#include <iostream>
#include <map>
#include <algorithm>
#include <mutex>
#include "play.hpp"


// Also update the recite method's implementation so that it compares the values of both the line_counter and scene_fragment_counter member variables, to the line number of the structured line to which the passed iterator refers and the passed scene fragment number, respectively.
// a function play the play
void
Play::recite(std::map<unsigned int, Structured_line>::const_iterator& it)
{
    // While the scene_fragment_counter member variable is less than the passed scene fragment number, or is equal but the line_counter member variable is less than the line number in the structured line referenced by the passed iterator, the recite method should repeatedly wait on a condition variable, until the line_counter and scene_fragment_counter member variables reach the values given in the the corresponding passed data.

    // When the scene_fragment_counter member variable equals the passed scene fragment number and the line_counter member variable equals the structured line's number, the recite method should print out the line (to cout, the standard output stream), increment the iterator, notify all other threads waiting on the condition variable, and return.


    // To avoid potential deadlocks and other problems if a badly formed script fragment is provided or somehow the program loses track of the count, if the scene_fragment_counter member variable is ever greater than the passed scene fragment number, or is equal but the line_counter member variable is greater than the line number in the structured line referenced by the passed iterator, the recite method should (1) write a line containing an error message to the standard error stream, (2) increment the iterator (but not the line counter), (3) notify all other threads waiting on the condition variable, and (4) return.



















    // check the condition variable and expect value to decide if it is the right time to stop wait
    std::unique_lock<std::mutex> lock(this->recite_mutex);
    this->recite_condv.wait(lock, [&](){
        return this->counter >= (*it).first;
    });

    // if the counter is ever greater than the structured line's number the recite method should:
    if (this->counter > (*it).first)
    {
        // 1.write a line containing an error message to the standard error stream (cerr, which is distinct from the cout standard output stream where properly ordered lines are written)
        std::cerr << "Error: couter is larger than lines_number" << std::endl;

        // 2. increment the iterator (but not the counter)
        it++;

        // 3. notify all other threads waiting on the condition variable
        lock.unlock();
        this->recite_condv.notify_all();

        // return
        return;

    }
    // when equal print out the line
    if (this->counter == (*it).first)
    {
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
        this->counter++;
        
        // notify all other threads waiting on the condition variable
        lock.unlock();
        this->recite_condv.notify_all();

        // return
        return;
    }

    // we don't need to deal with the situation of smaller since we will wait until it is equal or larger than counter
}


void 
Play::enter(unsigned int scence) 
{
    // if the passed value is less than the scene_fragment_counter member variable, the method should simply fail (by returning a non-zero error code, or throwing an exception, etc.); if the passed value is equal to the scene_fragment_counter member variable, the method should increment the on_stage member variable and return; if the passed value is greater than the scene_fragment_counter member variable, the method should wait on a condition variable until they are equal, and then increment the on_stage member variable and return.
}


void 
Play::exit()
{
    //(1) if the on_stage member variable is greater than 1 simply decrements the on_stage member variable; or (2) if the on_stage member variable is less than 1 throws an exception or returns a non-zero error code; or (3) if the on_stage member variable is exactly 1 then (a) decrements the on_stage member variable, (b) increments the scene_fragment_counter member variable, (c) if the iterator member variable is not already past the end of the container of scene titles prints out the string the iterator member variable currently references if that string is non-empty and then (whether or not the string was empty) increments the iterator member variable, and (d) signals the condition variable used by the enter method.
}