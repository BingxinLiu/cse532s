
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <set>

#include "director.hpp"
#include "utilities.hpp"

using namespace std;

const string delimit_token = "[scene]";
const size_t delimit_token_offset = delimit_token.size();

size_t
Director::parse_script_file(const string& script_config_file_name, size_t scene_config_index)
{
    // Get input file stream via config_file
    ifstream config_file_ifs(script_config_file_name);
    if ( !config_file_ifs.good() )
    {
        throw invalid_argument(script_config_file_name);
    }

    string line;
    set<character_name> character_names;
    while ( getline(config_file_ifs, line) )
    {
        line = trim(line);
        // skip empty lines or lines with only whitespace 
        if ( line.empty() ) continue;

        string character_name;
        string script_file_name;

        // for each configuration file that it can open should (1) count how many part definition lines each configuration file contains, (2) keep track of the maximum sum of the numbers of part configuration lines that appear in any two consecutive configuration files, and (3) when it reaches a configuration file line that immediately follows another configuration file line (i.e., does not immediately follow a new scene line that begins with the [scene] token) should push an empty string into container of scene titles.
        if ( istringstream(line) >> character_name >> script_file_name )
        {
            // check if current character has been inserted into set
            if ( character_names.find(character_name) == character_names.end())
            {
                cerr << "WARNNING: trying to insert duplicated scharacter's script file in a scene" << endl;
                continue;
            }
            character_names.insert(character_name);
            this->config[scene_config_index].second.pushback({character_name, script_file_name});
            

        } else 
        {
            cerr << "Error: can not parse line:\n" \
                 << "\t" << line << "\n" \
                 << "in file: " << script_config_file_name \
                 << endl;
            continue;
        }
    }
    return character_names.size();
}

// TODO: (2) keep track of the maximum sum of the numbers of part configuration lines that appear in any two consecutive configuration files, and (3) when it reaches a configuration file line that immediately follows another configuration file line (i.e., does not immediately follow a new scene line that begins with the [scene] token) should push an empty string into container of scene titles.
size_t
Director::parse_config_file(const string& scene_config_file_name)
{
    // Get input file stream via config file
    ifstream config_file_ifs(scene_config_file_name);
    if ( !config_file_ifs.good() )
    {
        throw invalid_argument(scene_config_file_name);
    }

    set<scene_name> scene_names;
    string scene_name;
    string directory = get_path_to_directory(scene_config_file_name);
    string line;
    size_t current_scene_config_index = 0;

    while ( getline(config_file_ifs, line))
    {
        trim(line);
        // skip empty line
        if (line.empty()) continue;

        //debug
        cout << "parsing line: " << line << endl;

        // check if the line contains the name of the scene
        if (line[0] ==  '[' && \
            line.substr(0, delimit_token_offset) == delimit_token)
        {
            // Whenever a new scene appears in the script (indicated by a line that begins with [scene] followed by the scene's title), the constructor should store scene's title (i.e., the rest of the line after the [scene] token) into a C++ style string and push that string back into a container member variable that holds the titles of the scenes (each of which will be printed out by the Play class object whenever the scene changes.
            scene_name = trim(line.substr(delimit_token_offset));

            // check if trying to insert a duplicated scence
            if ( scene_names.find(scene_name) == scene_names.end())
            {
                cerr << "WARNING: trying to parse a duplicated sence : " << scene_name << endl;
                continue;
            }
            
            // construct a new scene's name and scene's config pair
            current_scene_config_index = this->config.size();
            this->config.push_back({scene_name, {}});
            scene_names.insert(scene_name);
            this->scenes_names.push_back(scene_name);
        } else
        {
            // the line should be config file for the scene
            // Lines of the script file that do not start with a [scene] token should be interpreted as containing names of configuration files for the various fragments of the script. The constructor should attempt to open each configuration file
            parse_script_file(line, current_scene_config_index);
            this->scenes_names.push_back("");
        }
    }
    cout << "parsing config file done!" << endl;
    return scene_names.size();
}

void
Director::recruit(size_t player_num)
{
    this->players.clear();
    for (size_t i = 0; i < player_num; ++i)
    {
        this->players.push_back(make_shared<Player>(this->play.get(), this));
    }
}


Director::Director(const string& script_file, unsigned int min_player_number)
{
    // The constructor should open the script file in an input file stream and read through it (if the script file cannot be opened the constructor should throw an exception).

    try
    {
        parse_config_file(script_file);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    // After it finishes reading the script file, the constructor should dynamically (i.e, using the new operator) allocate a Play object (passing a reference to the container of scene title strings into the Play constructor), and store a shared_ptr (or other appropriate C++ smart pointer) to that Play object in a member variable so that when the Director is destroyed so is the Play.
    this->play = make_shared<Play>(this->config, this->scenes_names);

    // The Director class constructor should then take the maximum of the passed unsigned integer value and the maximum number of part lines in two consecutive script fragments, and should dynamically allocate (i.e, again using the new operator) that many Player objects (passing each one a reference to the Play object, and should push back a shared_ptr to each one into a container member variable of the Director class.
    recruit(min_player_number);
}

void 
Director::cue()
{
    // a cue method that can repeatedly hand off the name of a character and the name of a part definition file for that character, and a scene fragment number, to a Player, and with the information they contain run the Player's read and act methods to perform that part within the play.

    // For either of these variations (or for any alternative approach you may come up with) a termination protocol is needed so that after the script file has been completed, the Player threads all terminate and the Director and Player objects and all other resources of the program are freed after which the entire program ends. For the approach combining the HS/HA and Active Object patterns, for example, it may be straightforward to use a special termination ACT that is passed to each Player object's queue, which would cause the Player's thread to exit, and the Director could then join with each of those threads (e.g., by calling each Player's exit method in which the join would occur as in lab 1. For the L/F approach, it may be appropriate to modify the Director class so that the leader can detect when there are no more parts to be played, it can simply end its own thread, let the next leader be elected which then also will terminate, etc.

}