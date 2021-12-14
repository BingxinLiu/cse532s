
#include "director.hpp"

#include <fstream>
#include <sstream>
#include <set>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"

#include "../utilities/utilities.hpp"

// constructe a director
// which is working as a connector
director::director(u_short port, std::string ip_address, int min_threads, std::vector<std::string> scripts_filename)
{
    size_t scenes_max_charactors_num = 0;
    // parse plays config
    for (std::vector<std::string>::iterator it = scripts_filename.begin();
        it != scripts_filename.end();
        ++it)
    {
        scenes_max_charactors_num = max(scenes_max_charactors_num, this->parse_play_file(*it));
    }

    this->min_players = max((size_t)min_threads, scenes_max_charactors_num);

    if (DEBUG)
        this->print_configs();

    *safe_io << "Parsing plays config files done...", safe_io->flush();
    

    ACE_TCHAR buffer[BUFFER_SIZE];

    ACE_INET_Addr address(port, ip_address.c_str());
    ACE_SOCK_Connector connector;
    this->ace_sock_stream = new ACE_SOCK_Stream;

    if (connector.connect(*(this->ace_sock_stream), address) < 0)
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        *safe_io << "Error: Can not connect to " << buffer << ".";
        safe_io->flush();
        return;
    }

    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    *safe_io << "Start connect to " << buffer << " ...";
    safe_io->flush();
}


// register self as a event handler
// also send the register message to the producer to get a id
void
director::regis_self()
{
    ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
    //send [director_id:0] to apply a new director_id from producer
    std::stringstream ss;
    ss << "[director_id] " << this->director_id;
    if (ace_sock_stream->send_n(ss.str().c_str(), ss.str().length()+1) < 0 )
        std::cout << "Error: Can not send [director_id:0]" << std::endl;
    if (DEBUG)
        *safe_io << "SEND [" << ss.str() << "]", safe_io->flush();
}


// Register supported plays for producer
void
director::send_play_list()
{
    for (std::map<play_name, Config_struct>::iterator it = this->configs.begin();
        it != this->configs.end();
        ++it)
    {
        std::stringstream ss;
        ss << "[PLAY] " << this->director_id << " " << it->first;
        if (this->ace_sock_stream->send_n(ss.str().c_str(), ss.str().length() + 1) < 0)
            *safe_io << "Error: send " << ss.str() << " failed.", safe_io->flush();
    }
}


ACE_HANDLE 
director::get_handle() const
{
    return this->ace_sock_stream->get_handle();
}

// listen and parse received message from producer
int
director::handle_input(ACE_HANDLE h)
{
    char buffer[BUFFER_SIZE];
    size_t recv_len = 0;

    recv_len = this->ace_sock_stream->recv(&buffer, BUFFER_SIZE);
    if (recv_len <= 0)
    {
        int ret;
        ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
        if ( ret < 0)
        *safe_io << "remove handler failed", safe_io->flush();
        ret = this->ace_sock_stream->close();
        if ( ret < 0)
            *safe_io << "close socket failed", safe_io->flush();
        return EXIT_FAILURE;
    }

    this->parse_receive_msg(std::string(buffer));

    return EXIT_SUCCESS;

}

// handle close, release resources.
int
director::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    if (DEBUG)
        *safe_io << "handle close in reader ", safe_io->flush();
    if ( mask & READ_MASK )
    {
        if (DEBUG)
            *safe_io << "with READ_MASK\n";
        while (!this->play->finished)
        {
            this_thread::yield();
        }
        if (DEBUG)
            *safe_io << "This play has stopped", safe_io->flush();
        this->players.clear();
        if (DEBUG)
            *safe_io << "All players have left.", safe_io->flush();
        
        delete this->ace_sock_stream;
        delete this;
    }
    
    return EXIT_SUCCESS;
}

// handle user Ctrl-C signal
// sned confirm message and close self
int
director::handle_signal(int signal, siginfo_t* sig, ucontext_t* ucontx)
{
    *safe_io << "handle signal";
    if (this->play != nullptr)
        this->play->finished = true;
    safe_io->flush();
    std::stringstream ss;
    ss << QUIT_CONFIRM << " " << this->get_id();
    this->send_msg(ss.str());
    if (DEBUG)
        std::cout << "SEND [" << ss.str() << "]" << std::endl;
    ACE_Reactor::instance()->end_event_loop();
    ACE_Reactor::instance()->close();
    return EXIT_SUCCESS;
}

// parse play file
// return the maximum number of players in a scene
size_t
director::parse_play_file(const std::string& play_config_file_name)
{
    size_t max_characters_in_scenes = 0;
    try
    { 
        max_characters_in_scenes = this->parse_config_file(play_config_file_name);
    }
    catch(const std::exception& e)
    {
        std::cerr << "EXCEPTION in parse_play_file" << e.what() << '\n';
    }
    
    
    return max_characters_in_scenes;
}

// parse each fragment's config file
// record them into configs
// return the maximum number of players in a fragment
size_t
director::parse_script_file(const std::string& play_config_file_name, const std::string& script_config_file_name, size_t part_config_index)
{
    // Get input file stream via config_file
    std::ifstream config_file_ifs(script_config_file_name);
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

        if ( istringstream(line) >> character_name >> script_file_name )
        {
            // check if current character has been inserted into set
            if ( character_names.find(character_name) != character_names.end())
            {
                cerr << "WARNNING: trying to insert duplicated character's script file in a scene" << endl;
                continue;
            }
            character_names.insert(character_name);
            this->configs[play_config_file_name][part_config_index].second.push_back({character_name, script_file_name});
            

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

// parse each scene's config files
// recorde fragments info into configs
// return the maximum number of players in a scene
size_t
director::parse_config_file(const string& scene_config_file_name)
{
    // Get input file stream via config file
    std::ifstream config_file_ifs(scene_config_file_name);
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
            current_part_config_index = this->configs[scene_config_file_name].size();
            this->configs[scene_config_file_name].push_back({scene_name, {}});
            // (1) count how many part definition lines each configuration file contains,

            // (2) keep track of the maximum sum of the numbers of part configuration lines that appear in any two consecutive configuration files, and 
            // TODO: do not hard code
            current_scene_characters_num = parse_script_file(scene_config_file_name, line, current_part_config_index);
            max_scene_characters_num = max(max_scene_characters_num, current_scene_characters_num + last_scene_characters_num);
            last_scene_characters_num = current_scene_characters_num;
        }
    }

    // (3) when it reaches a configuration file line that immediately follows another configuration file line (i.e., does not immediately follow a new scene line that begins with the [scene] token) should push an empty string into container of scene titles.
    string current_scene_name = "";
    for ( Config_struct::iterator scene_it = this->configs[scene_config_file_name].begin();
          scene_it != this->configs[scene_config_file_name].end();
          ++scene_it )
    {
        if ( (*scene_it).first != current_scene_name )
        {
            current_scene_name = (*scene_it).first;
            this->scenes_names[scene_config_file_name].push_back(current_scene_name);
        } else
        {
            this->scenes_names[scene_config_file_name].push_back("");
        }

    }
    return max_scene_characters_num;
}

// parse received message. Do corresponding operation
void
director::parse_receive_msg(std::string str)
{
    std::stringstream ss(str);

    std::string command;

    if (ss >> command)
    {
        // GET id, then send play lists
        if (command == ID_COMMAND)
        {
            uint id;
            if (ss >> id, id > 0)
            {
                this->director_id = id;
                this->send_play_list();
            }
        }
        
        // GET start command
        // get the play name, recruite players, and start play
        if (command == START_COMMAND)
        {
            std::string playname;
            if (ss >> playname)
            {
                *safe_io << "PLAY " << playname;
                safe_io->flush();
                this->start_play(playname, this->min_players);
            }
        }

        // GET stop command
        // set the play as finished, notify all players to leave
        if (command == STOP_COMMAND)
        {
            std::string playname;
            if (ss >> playname && playname == this->play->playname)
            {
                *safe_io << "STOP " << playname;
                safe_io->flush();
                this->play->finished = true;
                this->play->needed_player_cv.notify_all();
                this->play = nullptr;
                std::stringstream ss;
                ss << STOPPED_CONFIRM << " " << this->get_id() << " " << playname;
                this->send_msg(ss.str());
                if (DEBUG)
                    std::cout << "SEND [" << ss.str() << "]" << std::endl;
            }
        }

        // GET quit command
        // set the play as finished, notify all players to leave
        // stop the event loop, close self, and release resources
        if (command == QUIT_COMMAND)
        {
            std::string str;
            if (ss >> str && str.length() > 0)
            {
                *safe_io << "WARNING: remaining commad: " << str;
                safe_io->flush();
            }
            if (DEBUG)
                *safe_io << QUIT_COMMAND;
            safe_io->flush();
            if (this->play != nullptr)
            {
                this->play->finished = true;
                this->play->needed_player_cv.notify_all();
            }
            std::stringstream ss;
            ss << QUIT_CONFIRM << " " << this->get_id();
            this->send_msg(ss.str());
            if (DEBUG)
                std::cout << "SEND [" << ss.str() << "]" << std::endl;
            ACE_Reactor::instance()->end_event_loop();
            ACE_Reactor::instance()->close();
        }
    } else
    {
        *safe_io << "ERROR: Can not parse received message: " << ss.str(), safe_io->flush();
    }
}

// send message via asc_sock_stream
void
director::send_msg(const std::string msg)
{
    this->ace_sock_stream->send_n(msg.c_str(), msg.length() + 1);
}

// construct instance of play
// recruit needed players
// and start to play
// finally send a startted confirm message to producer
void
director::start_play(std::string playname, uint player_num)
{
    this->config = this->configs[playname];

    if (DEBUG)
    {
        *safe_io << "START TO PLAY\n";
        *safe_io << this->config_to_str();
        *safe_io << this->scenes_names_to_str(playname);
        safe_io->flush();
    }
    
    this->play = make_shared<Play>(this->config, this->scenes_names[playname], *this, playname);

    this->recruit(this->min_players);

    this->start();

    std::stringstream ss;
    ss << STARTED_CONFIRM << " " << this->get_id() << " " << playname;
    this->send_msg(ss.str());
}

// FOR test purpose
// construct a stringstream of configs
// return a string formatted configs
std::string
director::config_to_str()
{
    std::stringstream ss;
    Config_struct::iterator cit = this->config.begin();    
    while (cit != this->config.end())
    {
        ss << cit->first << "\n";
        scene_config_struct scs = cit->second;
        for (scene_config_struct::iterator scs_it = scs.begin();
            scs_it != scs.end();
            ++scs_it)
        {
            ss << "\t" << scs_it->first << scs_it->second << "\n";
        }
        cit++;
    }
    return ss.str();
}

// FOR test purpose
// construct a stringstream of scenes_names
// return a string formatted scenes_names
std::string
director::scenes_names_to_str(std::string playname)
{
    std::stringstream ss;
    ss << "SCENES NAMES\n";
    std::vector<std::string>::iterator vit = this->scenes_names[playname].begin();
    while (vit != this->scenes_names[playname].end())
    {
        ss << "\t - " << *vit << "\n";
        vit++;
    }
    return ss.str();
}



// recruit enough players
void
director::recruit(size_t player_num)
{
    // new/recruite enough players
    for (size_t i = 0; i < player_num; ++i)
    {
        this->players.push_back(make_shared<Player>(this->play, *this));

    }
}

// assign each players with its job in a fragment
void 
director::cue(unsigned int frag_index)
{
    // a cue method that can repeatedly hand off the name of a character and the name of a part definition file for that character, and a scene fragment number, to a Player, and with the information they contain run the Player's read and act methods to perform that part within the play.
    scene_config_struct scene_config = this->config[frag_index].second;

    list<shared_ptr<Player> >::iterator players_it = this->players.begin();

    for ( scene_config_struct::iterator it = scene_config.begin(); it != scene_config.end(); ++it )
    {
        while ( players_it != this->players.end() )
        {
            if ( (*players_it)->activated ) break;
            players_it++;
        }


        if ( players_it == players.end() )
        {
            cerr << "ERROR: can not find a player to perform." << endl;
            throw invalid_argument(to_string(frag_index));
        }
        // set the character and job to a player
        (*players_it)->input_file_name = (*it).second;
        if (DEBUG)
            *safe_io << "INPUT FILE :" << (*it).second, safe_io->flush() ;
        (*players_it)->current_scene_index = frag_index;
        if (DEBUG) 
            *safe_io << "CURRENT SCENE IDEX :" << frag_index, safe_io->flush() ;
        (*players_it)->character = (*it).first;
        if (DEBUG)
            *safe_io << "CHARACTER :" << (*it).first, safe_io->flush() ;
        
        {
            lock_guard<mutex> lock((*players_it)->ready_to_read_mutex);
            (*players_it)->ready_to_read = true;
        }

        (*players_it)->ready_to_read_cv.notify_all();
        players_it++;
    }
    
    // safety check make one on one job
    while ( players_it != this->players.end() )
    {
        if ( (*players_it)->activated ) break;
        players_it++;
    }
    if ( players_it != players.end() )
    {
        cerr << "ERROR: some recruited player don't have character." << endl;
        throw invalid_argument(to_string(frag_index));
    } 


}

// wait for players become ready
// start the player who should recite first.
void
director::start()
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

    // std::thread t = std::thread([this](){
        
    // });
    // t.detach();
}

director::~director() 
{}

// FOR test purpose
// print configs
void
director::print_configs()
{
    for (std::map<play_name, Config_struct>::iterator it = this->configs.begin();
        it != this->configs.end();
        ++it)
    {
        *safe_io << it->first << "\n";
        Config_struct cs = it->second;
        for (Config_struct::iterator cs_it = cs.begin();
            cs_it != cs.end();
            ++cs_it)
        {
            *safe_io << "\t" << cs_it->first << "\n";
            scene_config_struct scs = cs_it->second;
            for (scene_config_struct::iterator scs_it = scs.begin();
                scs_it != scs.end();
                ++scs_it)
            {
                *safe_io << "\t\t" << scs_it->first << scs_it->second << "\n";
            }
        }
    }
    safe_io->flush();
}