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
#define PATH

int
main(int argc, char* argv[])
{
    // check the arguments number
    if (argc != CORRECT_ARGS_NUMBER)
    {
        // If it is run without a configuration file name your program should again print out a useful syntax message
        // (before ending gracefully by returning a unique non-zero error code), like the following:
        std::cerr << "usage: " << argv[0] << "<configuration_file_name>" << std::endl;
        return E2BIG;
    }

    // use its first command line argument (argv[1]) to identify a configuration file, and make up input stream of config file
    std::ifstream config_file_is(argv[CONFIG_FILE_OFFSET]);
    if (!config_file_is.good())
    {
        std::cerr << "Error: Invalid config file" << std::endl;
        return EINVAL;
    }

    // get the path to the file
    size_t pos = std::string(argv[CONFIG_FILE_OFFSET]).find_last_of("\\/");
    std::string directory_path = (pos == std::string::npos) ? "" : std::string(argv[CONFIG_FILE_OFFSET]).substr(0, pos+1);

    // read one line at a time from config (ignoring any lines that only contain whitespace), storing play name
    std::string play_name;
    while (getline(config_file_is, play_name) && play_name.empty()) {}
    if (play_name.empty())
    {
        std::cerr << "Error: There is no valid play name." << std::endl;
        return EINVAL; 
    }

    // 1. construct a Play object using the name of the play;
    Play play(play_name);

    // 2. construct a Player object for each well formed character definition line
    // the program saw
    std::vector<Player> players;
    std::string config_line;
    std::string character_name;
    std::string input_file_name;
    while(getline(config_file_is, config_line))
    {
        // clean white space
        config_line = trim(config_line);
        if (config_line.empty()) continue;
        // get character's name and its' script file's name
        if (std::istringstream(config_line) >> character_name >> input_file_name)
        {
            // construct input file stream of script file
            std::ifstream input_file_is(directory_path + input_file_name);
            if (!input_file_is.good())
            {
                std::cerr << "Error: file [" << (directory_path + input_file_name) << "] can not open" << std::endl;
                continue;
            }
            // construct an instance of player class for this character, push it into a vector
            players.push_back(Player(play, character_name, input_file_is));
        } else
        {
            std::cerr << "Error: config line [" << config_line << "] has some problem" << std::endl;
        }
    }
    // close config file stream
    config_file_is.close();

    // 3. after all of the Player objects have been constructed, call each Player object's enter method, start worker trhead
    for (auto &player : players)
        player.enter();
    // 4. after the enter method has been called on all of the Player objects, call each Player object's exit method so that it will join with main thread when it has finished
    for (auto &player : players)
        player.exit();

    // 5. return 0 to indicate success, or return a uniquely identifying non-zero error code
    // if an exception was thrown or an error occurred.
    return SUCCESS;
}