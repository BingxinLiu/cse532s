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