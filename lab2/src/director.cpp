
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <set>
#include <mutex>
#include <algorithm>

#include "director.hpp"
#include "utilities.hpp"

using namespace std;

const string delimit_token = "[scene]";
const size_t delimit_token_offset = delimit_token.size();

size_t
Director::parse_script_file(const string& script_config_file_name, size_t part_config_index)
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
            if ( character_names.find(character_name) != character_names.end())
            {
                cerr << "WARNNING: trying to insert duplicated character's script file in a scene" << endl;
                continue;
            }
            character_names.insert(character_name);
            this->config[part_config_index].second.push_back({character_name, script_file_name});
            

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

// TODO:
size_t
Director::parse_config_file(const string& scene_config_file_name)
{
    // Get input file stream via config file
    ifstream config_file_ifs(scene_config_file_name);
    if ( !config_file_ifs.good() )
    {
        throw invalid_argument("ERROR: file " + scene_config_file_name + "is bad.");
    }

    set<scene_name> scene_names;
    string scene_name = "";
    string directory = get_path_to_directory(scene_config_file_name);
    string line;
    size_t current_part_config_index = 0;
    size_t current_scene_characters_num = 0;
    size_t last_scene_characters_num = 0;
    size_t max_scene_characters_num = 0;

    while ( getline(config_file_ifs, line))
    {
        trim(line);
        // skip empty line
        if (line.empty()) continue;

        // check if the line contains the name of the scene
        if (line[0] ==  '[' && \
            line.substr(0, delimit_token_offset) == delimit_token)
        {
            // Whenever a new scene appears in the script (indicated by a line that begins with [scene] followed by the scene's title), the constructor should store scene's title (i.e., the rest of the line after the [scene] token) into a C++ style string and push that string back into a container member variable that holds the titles of the scenes (each of which will be printed out by the Play class object whenever the scene changes.
            scene_name = trim(line.substr(delimit_token_offset));

            // check if trying to insert a duplicated scence
            if ( scene_names.find(scene_name) != scene_names.end())
            {
                cerr << "WARNING: trying to parse a duplicated sence : " << scene_name << endl;
                continue;
            }
            
            
            scene_names.insert(scene_name);
        } else
        {
            
            // the line should be config file for the scene
            // Lines of the script file that do not start with a [scene] token should be interpreted as containing names of configuration files for the various fragments of the script. The constructor should attempt to open each configuration file
            // construct a new scene's name and scene's config pair
            current_part_config_index = this->config.size();
            this->config.push_back({scene_name, {}});
            // (1) count how many part definition lines each configuration file contains,

            // (2) keep track of the maximum sum of the numbers of part configuration lines that appear in any two consecutive configuration files, and 
            // TODO: do not hard code
            current_scene_characters_num = parse_script_file(directory + line, current_part_config_index);
            max_scene_characters_num = max(max_scene_characters_num, current_scene_characters_num + last_scene_characters_num);
            last_scene_characters_num = current_scene_characters_num;


            //this->scenes_names.push_back("");
        }
    }

    // (3) when it reaches a configuration file line that immediately follows another configuration file line (i.e., does not immediately follow a new scene line that begins with the [scene] token) should push an empty string into container of scene titles.
    string current_scene_name = "";
    for ( Config_struct::iterator scene_it = this->config.begin();
          scene_it != this->config.end();
          ++scene_it )
    {
        if ( (*scene_it).first != current_scene_name )
        {
            current_scene_name = (*scene_it).first;
            this->scenes_names.push_back(current_scene_name);
        } else
        {
            this->scenes_names.push_back("");
        }

    }
    return max_scene_characters_num;
}

void
Director::recruit(size_t player_num)
{
    //this->players.clear();
    for (size_t i = 0; i < player_num; ++i)
    {
        this->players.push_back(make_shared<Player>(this->play, *this));

    }
}


Director::Director(const string& script_file, unsigned int min_player_number)
{
    // The constructor should open the script file in an input file stream and read through it (if the script file cannot be opened the constructor should throw an exception).

    size_t max_characters_in_two_scene = 0;
    try
    {
        max_characters_in_two_scene = parse_config_file(script_file);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    if ( max_characters_in_two_scene > UINT64_MAX )
    {
        throw invalid_argument("ERROR: max characters in twon scene is too large.");
    }

    // After it finishes reading the script file, the constructor should dynamically (i.e, using the new operator) allocate a Play object (passing a reference to the container of scene title strings into the Play constructor), and store a shared_ptr (or other appropriate C++ smart pointer) to that Play object in a member variable so that when the Director is destroyed so is the Play.
    this->play = make_shared<Play>(this->config, this->scenes_names);

    // The Director class constructor should then take the maximum of the passed unsigned integer value and the maximum number of part lines in two consecutive script fragments, and should dynamically allocate (i.e, again using the new operator) that many Player objects (passing each one a reference to the Play object, and should push back a shared_ptr to each one into a container member variable of the Director class.
    recruit(max(static_cast<unsigned int>(max_characters_in_two_scene), min_player_number));

    this->start();
}

void 
Director::cue(unsigned int frag_index)
{
    // a cue method that can repeatedly hand off the name of a character and the name of a part definition file for that character, and a scene fragment number, to a Player, and with the information they contain run the Player's read and act methods to perform that part within the play.
    scene_config_struct scene_config = this->config[frag_index].second;

    list<shared_ptr<Player> >::iterator players_it = this->players.begin();

    for ( scene_config_struct::iterator it = scene_config.begin(); it != scene_config.end(); ++it )
    {
        while ( players_it != this->players.end() )
        {
            if ( (*players_it)->activated ) 
            {
                //cout << "+";
                break;
            }
            //cout << "-";
            players_it++;
        }

        // players_it = find_if(players_it, this->players.end(), [&](shared_ptr<Player> player){
        //     if (player->activated) cout << "+";
        //     else cout << "-";
        //     return player->activated;
        // });

        if ( players_it == players.end() )
        {
            cerr << "ERROR: can not find a player to perform." << endl;
            throw invalid_argument(to_string(frag_index));
        }
        // TODO don't hard code
        (*players_it)->input_file_name = "./script_files/" + (*it).second;
        (*players_it)->current_scene_index = frag_index;
        (*players_it)->character = (*it).first;
        {
            lock_guard<mutex> lock((*players_it)->ready_to_read_mutex);
            (*players_it)->ready_to_read = true;

        }
        (*players_it)->ready_to_read_cv.notify_all();
        players_it++;
    }
    
    // debug
    // players_it = find_if(players_it, players.end(), [](shared_ptr<Player> player){
    //     if (player->activated) cout << "+";
    //         else cout << "-";
    //         return player->activated;
    //     });
    while ( players_it != this->players.end() )
    {
        if ( (*players_it)->activated ) 
        {
            //cout << "+";
            break;
        }
        //cout << "-";
        players_it++;
    }
    if ( players_it != players.end() )
    {
        cerr << "ERROR: some recruited player don't have character." << endl;
        throw invalid_argument(to_string(frag_index));
    } 

    // For either of these variations (or for any alternative approach you may come up with) a termination protocol is needed so that after the script file has been completed, the Player threads all terminate and the Director and Player objects and all other resources of the program are freed after which the entire program ends. For the approach combining the HS/HA and Active Object patterns, for example, it may be straightforward to use a special termination ACT that is passed to each Player object's queue, which would cause the Player's thread to exit, and the Director could then join with each of those threads (e.g., by calling each Player's exit method in which the join would occur as in lab 1. For the L/F approach, it may be appropriate to modify the Director class so that the leader can detect when there are no more parts to be played, it can simply end its own thread, let the next leader be elected which then also will terminate, etc.

}

shared_ptr<Director>
Director::get_shared_ptr()
{
    cout << "???" << endl;
    return shared_from_this();
}

void
Director::start()
{
    // after recruite enough players, set need number of players in the first scene
    {
        lock_guard<mutex> lock(this->play->needed_player_num_mutex);
        this->play->needed_player_num = this->config.front().second.size();
    }
    // set number of not ready players
    {
        lock_guard<mutex> lock(this->play->not_ready_players_num_mutex);
        this->play->not_ready_players_num = this->config.front().second.size();
    }
    
    // notify one player so that it can be the leader
    this->play->needed_player_cv.notify_one();
}

Director::~Director() 
{
    while ( !this->play->finished ) 
    {
        this_thread::yield();
    }
    for (list<shared_ptr<Player> >::iterator it = this->players.begin();
    it != this->players.end(); ++it)
    {
        (*it)->exit();
    }
}

