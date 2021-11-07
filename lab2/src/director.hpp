
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <list>

#include "play.hpp"
#include "player.hpp"
#include "def.hpp"


using namespace std;

class Director
{
    Config_struct config;
    shared_ptr<Play> play;
    list<shared_ptr<Player> > players;

    vector<scene_name> scenes_names;
    size_t parse_script_file(const string& script_config_file_name, size_t scene_config_index);
    size_t parse_config_file(const string& scene_config_file_name);
    void recruit(size_t player_num);

public:
    //constructor
    Director(const string& script_file_name, unsigned int min_player_number){};

    void cue(){};
};