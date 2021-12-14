#include <sstream>
#include <utility>
#include "threadsafe_menu.hpp"

// construct a string for this menu for UI service or the need to print out the menu
// format:
/*
Index   Name    Status[availabe numbers]
*/
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
        if (DEBUG)
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

// register a play into the menu
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

// pop a director's id that can play the specified play
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

// pop a busy director's id that palying the specified play
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

// NOTE: we have a gray area that play/stop command sent but have not received reply. This status we call it as UNCLEAR

// set the director as idle with id
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

// set the director as busy with id
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

// clear a director's record in this menu
void
threadsafe_menu::clean_with_id(uint id)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    if (this->menu.find(id) != this->menu.end())
        this->menu.erase(this->menu.find(id));
}

// check if the menu is empty
// return the number of exist directors
size_t
threadsafe_menu::is_empty()
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    return this->menu.size();
}

// get the play name according to the index of the menu
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