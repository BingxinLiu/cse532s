#include <iostream>
#include <map>
#include <algorithm>
#include <mutex>
#include "play.hpp"

void
Play::recite(std::map<unsigned int, Structured_line>::const_iterator& it)
{
    /**
     * @brief 
     * Todo:
     * As in the print method of the lab 0 assignment, to ensure that lines are
     *  associated with the proper characters, as it goes the recite method of the
     *  Play class should also keep track of which character is currently speaking,
     *  and when a change occurs should print out (1) a blank line and (2) a line with 
     * the name of the new character (with a period at the end) before printing out 
     * the new character's subsequent line(s).
     */
    std::unique_lock<std::mutex> lock(this->recite_mutex);
    this->recite_condv.wait(lock, [&](){
        return this->counter >= (*it).first;
    });
    if (this->counter > (*it).first)
    {
        // if the counter is ever greater than the structured line's number the recite method should:

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

//    // compare the value of the counter member variable with the line number of structed line to which the iterator refers
//    if (this->counter < (*it).lines_number)
//    {
//        // repeatedly wait on a condition varibale
//        while(condition variable)
//        {
//            // do something
//        }
//        // until reaches the lines number
//    }
    if (this->counter == (*it).first)
    {
        if (current_character != (*it).second.character)
        {
            std::cout << "\n" << (*it).second.character << "." << std::endl;
            current_character = (*it).second.character;
        }

        // when equal print out the line
        std::cout << (*it).second.text << std::endl;
        // increment the iterator
        it++;

        // increment the counter once per line that is delivered, but whether you increment it before or after the line is delivered is up to you (e.g., according to whether you chose to initialize the counter to 0 or 1).
        this->counter++;
        
        // notify all other threads waiting on the condition variable
        lock.unlock();
        this->recite_condv.notify_all();

        // return
        return;
    } else
    {
        // Something wrong
    }
}


