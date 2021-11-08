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
#define MINIMUM_CORRECT_ARGS_NUM 2
#define MAXIMUM_CORRECT_ARGS_NUM 3
#define CONFIG_FILE_OFFSET 1
#define MIN_PLAYER_NUM_OFFSET 2
#define PATH

int
main(int argc, char* argv[])
{
    // check the arguments number
    if ( argc < MINIMUM_CORRECT_ARGS_NUM || argc > MAXIMUM_CORRECT_ARGS_NUM )
    {
        // If it is run without a configuration file name your program should again print out a useful syntax message
        // (before ending gracefully by returning a unique non-zero error code), like the following:
        cerr << "USAGE: " << argv[0] << "<script_file_name> [<minimum number of players to use>]" << endl;
        return EINVAL;
    }

    // use its first command line argument (argv[1]) to identify a configuration file, and make up input stream of config file
    ifstream config_file_is(argv[CONFIG_FILE_OFFSET]);
    if (!config_file_is.good())
    {
        cerr << "Error: Invalid config file" << endl;
        return EINVAL;
    }

    unsigned int min_plyer_num;
    // if a second command line argument was given (in argv[2]) should treat that argument as an unsigned integer giving the minimum number of players to use in performing the play
    if ( argc > MINIMUM_CORRECT_ARGS_NUM)
    {
        int arg;
        try 
        {
            arg = stoi(argv[MIN_PLAYER_NUM_OFFSET]);
        }
        catch(...)
        {
            cerr << "Error: The third argument should be an integer." << endl;
            cerr <<  "USAGE: " << argv[0] << "<script_file_name> [<minimum number of players to use>]" << endl;
            return EINVAL;
        }
        if (arg <= 0)
        {
            cerr << "Error: The third argument should be a positive integer." << endl;
            return EINVAL;
        }
        min_plyer_num = arg;
    }

    // get the path to the file
    // TODO: maybe useless
    size_t pos = std::string(argv[CONFIG_FILE_OFFSET]).find_last_of("\\/");
    std::string directory_path = (pos == std::string::npos) ? "" : std::string(argv[CONFIG_FILE_OFFSET]).substr(0, pos+1);

    Director director(argv[CONFIG_FILE_OFFSET], min_plyer_num);

    /*
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
    */
    return SUCCESS;
}