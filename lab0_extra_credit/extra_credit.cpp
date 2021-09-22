#include <iostream>
#include <fstream>
#include <string>
#include <set>

#define ZERO 0

int
main(int argc, char * argv[])
{
    if (argc < 3)
    {
        std::cout << "usage: " << argv[0]\
            << " <the name of a script fragment file to process>.txt"\
            << " <the name of a configuration file to generate>.txt"\
            << " <the name of the play>" << std::endl;
        return EINVAL;
    }
    std::string script_file_name(argv[1]);
    std::string configuration_file_name(argv[2]);
    // Read play name
    std::string play_name("");
    for (int i = 3; i < argc; ++i)
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
                ofs << line_index;
                if (line_index++ < 10) ofs << " ";
                ofs << " " << line << std::endl;
            }
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
    return ZERO;
    
}