#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <cstring>

#define ZERO 0
#define SUCCESS 0
#define SCRIPT_FILE_POS 1
#define CONFIG_FILE_POS 2
#define PLAY_NAME_START_POS 3
#define SCRAMBLE_POS 1
#define MIN_ARGC 3
#define SHIFT 1

int
main(int argc, char * argv[])
{
    if (argc < MIN_ARGC)
    {
        std::cout << "usage: " << argv[0]\
            << " [SCRAMBLE] "\
            << " <the name of a script fragment file to process>"\
            << " <the name of a configuration file to generate>"\
            << " <the name of the play>" << std::endl;
        return EINVAL;
    }
    bool scramble_flag = false;
    if ( !strcmp(argv[SCRAMBLE_POS], "SCRAMBLE") )  scramble_flag = true;
    std::string script_file_name(argv[SCRIPT_FILE_POS + (scramble_flag? SHIFT : ZERO)]);
    std::string configuration_file_name(argv[CONFIG_FILE_POS + (scramble_flag? SHIFT : ZERO)]);
    // Read play name
    std::string play_name("");
    for (int i = PLAY_NAME_START_POS + (scramble_flag? SHIFT : ZERO); i < argc; ++i)
    {
        play_name += argv[i];
        if (i < argc - 1) play_name += " ";
    }
    
    std::set<std::string> character_set;
    std::ifstream input_file(script_file_name);
    std::string line;
    std::string current_character("");
    int line_index = 1;
    if (!input_file.is_open())
    {
        std::cout << "Error: Cannot open file: " << script_file_name << "." << std::endl;
        return EINVAL;
    }
    while(std::getline(input_file, line))
    {
        // skip empty line and spaces-only line
        if (line.find_first_not_of(" ") >= line.size() || line.size() == 0)
            continue;
        // trim line
        line.erase(0, line.find_first_not_of(" "));

        // read character name, put lines under the name into specificed file
        if (line.find(".") < line.size() && current_character.compare(line.substr(0, line.find("."))) != 0)
        {
            current_character = line.substr(0, line.find("."));
            character_set.insert(current_character);
            std::vector<std::string> lines;
            std::string output_file = current_character + ".txt";
            std::ofstream ofs(output_file, std::ios::out | std::ios::app);
            if (!ofs.is_open())
            {
                std::cout << "Error: Cannot open file: " << output_file << "." << std::endl;
                continue;
            }
            while(std::getline(input_file, line))
            {
                if (line.size() == 0) break;
                std::string structured_line;
                structured_line += std::to_string(line_index);
                // Padding, like "1  XXX" and "11 XXX"
                if (line_index++ < 10) structured_line += " ";
                structured_line += " ";
                structured_line += line;
                structured_line += "\n";
                lines.push_back(structured_line);
            }
            // shuffle lines, learnt from c++ reference
            std::srand( unsigned ( std::time(0) ) );
            std::random_shuffle(lines.begin(), lines.end());

            // Write shuffled lines into file
            for (auto aline : lines)
                ofs << aline;

            ofs.close();
        }
        continue;
    }

    // build up configure.txt
    std::ofstream ofs(configuration_file_name);
    if (!ofs.is_open())
    {
        std::cout << "Error: Cannot open file: " << configuration_file_name << "." << std::endl;
        return EINVAL;
    }
    ofs << play_name << std::endl;
    for (std::set<std::string>::iterator it = character_set.begin();\
        it != character_set.end(); ++it)
    {
        ofs << (*it) << " " << (*it) << ".txt" << std::endl;
    }
    ofs.close();
    return SUCCESS;
    
}