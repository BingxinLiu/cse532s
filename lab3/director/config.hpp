#include <string>
#include <vector>

using namespace std;

typedef string character_name;
typedef string character_script_name;
typedef string scene_name;
typedef pair<character_name, character_script_name> character_name_script_pair;
typedef vector<character_name_script_pair> scene_config_struct;
typedef pair<scene_name, scene_config_struct> scene_name_config_pair;
typedef vector<scene_name_config_pair> Config_struct;

typedef string play_name;

// a structure for recording the config for the whole scene




// #include <string>
// #include <map>
// #include <vector>


// typedef std::string character_name;
// typedef std::string character_script_name;
// struct characters
// {
//     std::map<character_name, character_script_name> characters_config_;
//     character_script_name& operator[](const character_name character_name_)
//     {
//         return this->characters_config_[character_name_];
//     }
// };

// typedef std::string fragment_name;
// struct fragments
// {
//     std::map<fragment_name, characters> fragments_config_;
//     characters& operator[](const fragment_name fragment_name_)
//     {
//         return this->fragments_config_[fragment_name_];
//     }
// };

// typedef std::string scene_name;
// struct scenes
// {
//     std::map<scene_name, fragments> scenes_config_;
//     fragments& operator[](const scene_name scene_name_)
//     {
//         return this->scenes_config_[scene_name_];
//     }
// };

// typedef std::string play_name;

// class plays
// {
//     std::map<play_name, scenes> plays_config_;
//     scenes& operator[](const play_name play_name_)
//     {
//         return this->plays_config_[play_name_];
//     }
// };