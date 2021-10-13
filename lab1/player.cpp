#include <string>
#include <iostream>
#include <sstream>
#include "play.hpp"
#include "player.hpp"
#include "utilities.hpp"

void Player::read()
{
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
        // debug
        // std::cout << "read: " << line << std::endl;

        line = trim(line);
        if (line.empty()) continue;

        unsigned int order_number;
        std::string text;
        std::istringstream issed_line(line);

        // converts the first whitespace delimited token of the line into a number (e.g., an unsigned int);
        if (!(issed_line >> order_number))
        {
            std::cerr << "Error: can not cope with line : " << line << std::endl;
            continue;
        }

        // stores the rest of the line in a C++ style string
        if (!getline(issed_line, text))
        {
            std::cerr << "Error: can not cope with line : " << line << std::endl;
            continue;
        }
        text = trim(text);

        // if (and only if) both the number and some non-whitespace text were extracted from the line, inserts a structured line (based on the number, the character's name, and the text of the line following the number) into its container member variable (make sure that the container either preserves the order in which the lines appeared in the file, or reorders them by line number).
        // It is possible that the method may see no well formed lines, which also is a reasonable case (e.g., to allow silent characters to enter and leave a scene in later labs) as long as it does recognize well formed lines if there are any.
        this->lines.insert(std::pair<uint, Structured_line>(order_number, Structured_line(this->character, text)));
    }
    this->input_file_stream.close();
    //debug
    // std::cout << "Read stopped" << std::endl;
}

void Player::act()
{
    //first initializes an iterator positioned at the beginning of the container of lines that the read method loaded from the file stream;
    //it should then repeatedly pass the iterator into a call to the recite method of the Play class referenced by the private member variable, until the iterator is past the last structured line in the container.
    std::map<unsigned int, Structured_line>::const_iterator it = this->lines.begin();
    
    while (it != this->lines.end())
    {
        this->play.recite(std::ref(it));
    }
}

// Write a public enter method that uses move semantics to launch a new thread and transfer ownership of it into the std::thread member variable. The thread should call the read method and then the act method of the object on which the enter method was called (hint: a lambda expression may be helpful to do this).
void
Player::enter()
{
    this->mythread = std::thread([this](){
        this->read();
        this->act();
    });
}

// Write a public exit method that tests whether or not the std::thread member variable is joinable and if (and only if) it is calls its join method.
void
Player::exit()
{
    if (this->mythread.joinable()) {
        this->mythread.join();
    }
}