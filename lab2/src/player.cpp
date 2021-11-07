#include <string>
#include <iostream>
#include <sstream>
#include "play.hpp"
#include "player.hpp"
#include "utilities.hpp"


// Update the read method so that each time it is called it first empties the container of structured lines that is stored by the Player and then re-populates the container with lines from the character part file stream.
// a public member function read each line of its scripts and store as a structed line
void Player::read()
{
    // check if the input file stream is valid
    if(!this->input_file_stream.good())
    {
        std::cerr << "Error: Input file stream broken. [ " << this->character << " ]" << std::endl;
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
        
        this->lines.insert(std::pair<uint, Structured_line>(order_number, Structured_line(this->character, text)));
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
        this->play.recite(std::ref(it));
    }
}


//Update the enter and exit methods so that they are compatible with the appropriate thread pool management semantics for the HS/HA or L/F design approach you chose. In particular, issues of thread construction, thread spawning, and thread join or detach semantics should be considered for these methods.

//a public enter method that uses move semantics to launch a new thread and transfer ownership of it into the std::thread member variable. The thread should call the read method and then the act method of the object on which the enter method was called.
void
Player::enter()
{
    this->mythread = std::thread([this](){
        this->read();
        this->act();
    });
}

// a public exit method that tests whether or not the std::thread member variable is joinable and if (and only if) it is calls its join method.
void
Player::exit()
{
    if (this->mythread.joinable()) {
        this->mythread.join();
    }
}