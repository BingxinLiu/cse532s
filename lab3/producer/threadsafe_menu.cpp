#include <sstream>
#include <utility>
#include "threadsafe_menu.hpp"

const std::string
threadsafe_menu::str()
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    std::stringstream ss;
    ss << "Index\tName\t\tStatus\n";
    this->play_name_available_map.clear();
    for (menu_type::iterator mit = this->menu.begin();
        mit != this->menu.end();
        ++mit)
    {
        *safe_io << "ID" << mit->first << " STATUS " << mit->second.first;
        for (play_names_type::iterator nit = mit->second.second.begin();
            nit != mit->second.second.end();
            nit++)
        {
            if (mit->second.first == AVAILABLE)
            {
                if (this->play_name_available_map.find(*nit) == this->play_name_available_map.end())
                {
                    this->play_name_available_map[*nit] = 1;
                } else
                {
                    this->play_name_available_map[*nit]++;
                }
            } else
            {
                if (this->play_name_available_map.find(*nit) == this->play_name_available_map.end())
                {
                    this->play_name_available_map[*nit] = 0;
                }
            }

        }
    }
    for (std::map<play_name, size_t>::iterator mit = this->play_name_available_map.begin();
        mit != this->play_name_available_map.end();
        ++mit)
    {
        ss << std::distance(this->play_name_available_map.begin(), mit) << "\t" << mit->first << "\t";
        if (mit->second > 0)
        {
            ss << "AVAILABLE [ " << mit->second << " ]\n";
        } else
        {
            ss << "UNAVAIBALE\n";
        }
    }
    return ss.str();
}


void
threadsafe_menu::reg_play(play_name play_name_, director_id id)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    if (this->menu.find(id) != this->menu.end())
    {
        this->menu[id].second.insert(play_name_);
    } else
    {
        std::vector<play_name> v{play_name_};
        this->menu[id] = {AVAILABLE, {play_name_}};
    }
}

uint
threadsafe_menu::pop_avaliable(play_name play_name_)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    for (menu_type::iterator mit = this->menu.begin();
        mit != this->menu.end();
        ++mit)
    {
        // if this director available, find the play name in its set
        if (mit->second.first == AVAILABLE)
        {
            for (play_names_type::iterator nit = mit->second.second.begin();
                nit != mit->second.second.end();
                nit++)
            {
                if (*nit == play_name_)
                {
                    mit->second.first = UNCLEAR;
                    return mit->first;
                }
            }
        }
    }
    return NONE;
}

uint
threadsafe_menu::pop_busy_play(play_name play_name_)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    for (menu_type::iterator mit = this->menu.begin();
        mit != this->menu.end();
        ++mit)
    {
        if (mit->second.first == UNAVAILABLE)
        {
            for (play_names_type::iterator nit = mit->second.second.begin();
                nit != mit->second.second.end();
                nit++)
            {
                if (*nit == play_name_)
                {
                    mit->second.first = UNCLEAR;
                    return mit->first;
                }
            }
        }
    }
    return NONE;
}


void 
threadsafe_menu::set_idle_with_id(uint id)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    std::stringstream ss;
    if (this->menu.find(id) == this->menu.end())
    {
        *safe_io << "ERROR: this id has not registed", safe_io->flush();
    } else
    {
        this->menu[id].first = AVAILABLE;
    }
}

void 
threadsafe_menu::set_busy_with_id(uint id)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    std::stringstream ss;
    if (this->menu.find(id) == this->menu.end())
    {
        *safe_io << "ERROR: this id has not registed", safe_io->flush();
    } else
    {
        this->menu[id].first = UNAVAILABLE;
    }
}

void
threadsafe_menu::clean_with_id(uint id)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    if (this->menu.find(id) != this->menu.end())
        this->menu.erase(this->menu.find(id));
}

size_t
threadsafe_menu::is_empty()
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    return this->menu.size();
}

play_name
threadsafe_menu::operator[](uint offset)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    if (offset > this->play_name_available_map.size()) return "";
    std::map<play_name, size_t>::iterator it = this->play_name_available_map.begin();
    for (uint i = 0; i < offset; ++i)
    {
        it++;
    }
    return it->first;
}