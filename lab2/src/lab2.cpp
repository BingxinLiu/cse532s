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
    size_t pos = std::string(argv[CONFIG_FILE_OFFSET]).find_last_of("\\/");
    std::string directory_path = (pos == std::string::npos) ? "" : std::string(argv[CONFIG_FILE_OFFSET]).substr(0, pos+1);

    // construct a dirctor it will start the play
    Director* director = new Director(argv[CONFIG_FILE_OFFSET], min_plyer_num);
    delete director;


    return SUCCESS;
}