#include <iostream>
#include <string>
#include <map>
#include <cerrno>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>

#define ZERO 0

typedef std::ifstream script_file_ifstream;
typedef std::string character_name;
typedef std::string text;
typedef int index;
typedef std::pair<character_name, text> name_text_pair;
typedef std::map<index, name_text_pair> structuered_lines;

struct Structured_line
{
    index idx_;
    name_text_pair pair_;
    Structured_line(index idx, name_text_pair pair) : idx_(idx), pair_(pair) {}
};
class Play
{
    std::string play_name_;
    structuered_lines structured_lines_;
    mutable std::mutex m;
public:
    Play(std::string play_name) : play_name_(play_name) {}
    Play& operator=(const Play&) = delete;

    Play& operator<< (Structured_line line);
    void print(std::ostream& os);

};
class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_):t(t_) {}
    ~thread_guard()
    {
        if(t.joinable()) t.join();
    }
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};

void read_thread(Play& play, character_name const & name, std::istream& is);
bool arguments_sanity_check(int, char **);


int main
(int argc, char * argv[])
{
    // Check the sanity of input arguments
    if (!arguments_sanity_check(argc, argv))
    {
        std::cout << "usage: " << argv[0] << " <configuration_file_name>.txt" << std::endl;
        return EINVAL;
    }

    // store character name and script file name
    std::map<character_name, script_file_ifstream> character_script_map = std::map<character_name, script_file_ifstream>();

    
    std::string line;
    std::string file_name;
    character_name name;
    bool first_line_have_read = false;
    std::string play_name;
    std::ifstream infile(argv[1]);
    // 
    if (!infile.is_open())
    {
        std::cout << "Error: File " << argv[1] << "can not open!" << std::endl;
        return EINVAL;
    }
    // Read configuration file line by line
    while (std::getline(infile, line))
    {
        // skip empty line
        if (line.size() == 0) continue;
        // store play name from first line
        if (!first_line_have_read)
        {
            play_name = line;
            first_line_have_read = true;
            continue;
        }
        // skip spaces-only line
        if (line.find_first_not_of(" ") >= line.size()) continue;
        // seperate line into character name and character script
        size_t pos = line.find(" ");
        name = line.substr(0, pos);
        file_name = line.substr(pos+1);
        if (file_name.size() == 0 || file_name.find_first_not_of(" ") >= file_name.size())
        {
            //debug
            std::cout << "Something wrong when reading configuration file " << line << std::endl;
            continue;
        }
        // open script file and store structured line into a map as value, and character name as key
        std::ifstream script_file(file_name);
        if(!script_file.is_open())
        {
            std::cout << "Error: File " << file_name << " can not open!" << std::endl;
            continue;
        }
        character_script_map[name] = std::move(script_file);
    }

    // construct a Play object using the name of the play
    Play play(play_name);

    // construct a std::thread object for each well formed character definition line
    for (std::map<character_name, script_file_ifstream>::iterator it = character_script_map.begin(); it != character_script_map.end();++it)
    {
        std::thread t(read_thread, std::ref(play), std::ref((*it).first), std::ref((*it).second));
        // join with each of the threads
        thread_guard g(t);
    }

    // call the Play object's print
    play.print(std::cout);
    
    return ZERO;
}


bool
arguments_sanity_check(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "The number of command line arguments is wrong!" << std::endl;
        return false;
    }
    std::string file_name = argv[1];
    if (file_name.compare(file_name.size() - 5, 4, ".txt") == 0)
    {
        std::cout << "The configuration should have a file with \".txt\" suffix!" << std::endl;
        return false;
    }
    return true;
}

void
read_thread(Play& play, character_name const & name, std::istream& is)
{
    std::string line;
    while (std::getline(is, line))
    {
        if (line.size() == 0) continue;
        if (line.find_first_not_of(" ") >= line.size()) continue;
        size_t pos = line.find(" ");
        int idx = std::stoi(line.substr(0, pos));
        line.erase(0, pos);
        if (line.find_first_not_of(" ") >= line.size()) continue;
        if (line.size() == 0) continue;
        line.erase(0, line.find_first_not_of(" "));
        play << std::move(Structured_line(idx, name_text_pair(name, line)));
    }

}



Play&
Play::operator<< (Structured_line line)
{
    std::lock_guard<std::mutex> lock(this->m);
    this->structured_lines_.insert(std::pair<index, name_text_pair>(line.idx_, line.pair_)); 
    return *this;
}

void
Play::print(std::ostream& os)
{
    os << this->play_name_ << "\n";
    character_name current_character;
    for (structuered_lines::iterator it = this->structured_lines_.begin(); it != this->structured_lines_.end(); ++it)
    {
        if(it == this->structured_lines_.begin() || current_character.compare((*it).second.first) != 0)
        {
            current_character = (*it).second.first;
            std::cout << "\n" << current_character << ". " << std::endl;
        }
        std::cout << (*it).second.second << std::endl;
    }
}