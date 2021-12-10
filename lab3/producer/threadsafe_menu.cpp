#include <sstream>
#include "threadsafe_menu.hpp"

const std::string
threadsafe_menu::str()
{
    std::stringstream ss;
    std::lock_guard<std::mutex> lk(this->menu_mutex);
    
    ss << "Index\tName\t\tStatus\n";
    for (std::list<std::pair<play_name, std::vector<director_id>>>::const_iterator cit = this->avaliable_items.cbegin();
        cit != this->avaliable_items.cend();
        ++cit)
    {
        ss << std::distance(this->avaliable_items.cbegin(), cit) << "\t" << cit->first << " ";
        if (cit->second.size() > 0)
        {
            ss << "Avaliable[" << cit->second.size() << "]";
        } else
        {
            ss << "Unavailable";
        }
        ss << "\n";
    }
    return ss.str();
}

void
threadsafe_menu::reg_play(play_name play_name_, director_id id)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    for (std::list<std::pair<play_name, std::vector<director_id>>>::iterator it = this->avaliable_items.begin();
        it != this->avaliable_items.end();
        ++it
    )
    {
        if (it->first == play_name_)
        {
            it->second.push_back(id);
            return;
        }       
    }
    this->avaliable_items.push_back({play_name_, {id}});
}

uint
threadsafe_menu::pop_avaliable(play_name play_name_)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    for (std::list<std::pair<play_name, std::vector<director_id>>>::iterator it = this->avaliable_items.begin();
        it != this->avaliable_items.end();
        ++it
    )
    {
        if (it->first == play_name_)
        {
            if (it->second.size() == 0)
            {
                return NONE;
            }
            else
            {
                uint id = it->second.back();
                it->second.pop_back();
                this->reg_busy_play(play_name_, id);
                return id;
            }
        }
    }

    return NONE;
}

void
threadsafe_menu::reg_busy_play(play_name play_name_, director_id id)
{
    for (std::list<std::pair<play_name, std::vector<director_id>>>::iterator it = this->busy_items.begin();
        it != this->busy_items.end();
        ++it
    )
    {
        if (it->first == play_name_)
        {
            it->second.push_back(id);
            return;
        }
    }
    this->busy_items.push_back({play_name_, {id}});
}

play_name
threadsafe_menu::operator[](uint offset)
{
    std::list<std::pair<play_name, std::vector<director_id>>>::iterator it = this->avaliable_items.begin();
    for (uint i = 0; i < offset; ++i)
    {
        it++;
    }
    return it->first;
}