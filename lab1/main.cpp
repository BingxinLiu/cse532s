#include <iostream>
#include <sstream>
#include <string>
#include <cerrno>
#include <fstream>
#include <vector>
#include "play.hpp"
#include "player.hpp"
#include "utilities.hpp"

#define SUCCESS 0
#define CORRECT_ARGS_NUMBER 2
#define CONFIG_FILE_OFFSET 1

int
main(int argc, char* argv[])
{
    // use its first command line argument (argv[1]) to identify a configuration file formatted like hamlet_ii_2_config.txt, 
    // and read one line at a time from it (ignoring any lines that only contain whitespace), storing important information
    // that it will use in the rest of its operation.

    if (argc != CORRECT_ARGS_NUMBER)
    {
        // If it is run without a configuration file name your program should again print out a useful syntax message
        // (before ending gracefully by returning a unique non-zero error code), like the following:
        std::cerr << "usage: " << argv[0] << "<configuration_file_name>" << std::endl;
        return E2BIG;
    }

    std::ifstream config_file_is(argv[CONFIG_FILE_OFFSET]);
    if (!config_file_is.good())
    {
        std::cerr << "Error: Invalid config file" << std::endl;
        return EINVAL;
    }

    std::string play_name;
    while (getline(config_file_is, play_name) && play_name.empty()) {}
    if (play_name.empty())
    {
        std::cerr << "Error: There is no valid play name." << std::endl;
        return EINVAL; 
    }

    //debug
    std::cout << "play name : " << play_name << std::endl;

    // 1. construct a Play object using the name of the play;
    Play play(play_name);

    // 2. construct a Player object for each well formed character definition line
    // the program saw, passing its constructor a reference to the Play object,
    // a reference to the C++ style string for the character's name, and a reference
    // to the input file stream object for that character part's input text file of lines;
    std::vector<Player> players;
    std::string config_line;
    std::string character_name;
    std::string input_file_name;
    while(getline(config_file_is, config_line))
    {
        config_line = trim(config_line);
        if (config_line.empty()) continue;
        if (std::istringstream(config_line) >> character_name >> input_file_name)
        {
            std::ifstream input_file_is(input_file_name);
            if (!input_file_is.good())
            {
                std::cerr << "Error: file [" << input_file_name << "] can not open" << std::endl;
                continue;
            }
            players.push_back(Player(play, character_name, input_file_is));
        } else
        {
            std::cerr << "Error: config line [" << config_line << "] has some problem" << std::endl;
        }
    }

    config_file_is.close();

    // 3. after all of the Player objects have been constructed, call each Player object's
    // enter method;
    for (auto &player : players)
        player.enter();
    // 4. after the enter method has been called on all of the Player objects, call each
    // Player object's exit method; and
    for (auto &player : players)
        player.exit();

    // 5. return 0 to indicate success, or return a uniquely identifying non-zero error code
    // if an exception was thrown or an error occurred.
    return SUCCESS;

    




    



}