#include <iostream>
#include <map>
#include <algorithm>
#include <mutex>
#include "play.hpp"


// a function play the play
void
Play::recite(std::map<unsigned int, Structured_line>::const_iterator& it)
{
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


