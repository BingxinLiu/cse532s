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
    *safe_io << "PARSE MENU DONE", safe_io->flush();
    for (std::map<play_name, size_t>::iterator mit = this->play_name_available_map.begin();
        mit != this->play_name_available_map.end();
        ++mit)
    {
        *safe_io << "CONSRTUCT" << mit->first, safe_io->flush();
        ss << std::distance(this->play_name_available_map.begin(), mit) << "\t" << mit->first << "\t";
        *safe_io << "??", safe_io->flush();
        if (mit->second > 0)
        {
            ss << "AVAILABLE [ " << mit->second << " ]\n";
        } else
        {
            ss << "UNAVAIBALE\n";
        }
        *safe_io << "?", safe_io->flush();
    }
    *safe_io << "CONSRTUCT UI MENU DONE", safe_io->flush();
    return ss.str();
}

/*
const std::string
threadsafe_menu::test_str()
{
    std::stringstream ss;
    std::lock_guard<std::mutex> lk(this->menu_mutex);
    
    ss << "Index\tName\t\tStatus\n";
    for (std::list<std::pair<play_name, std::vector<director_id>>>::const_iterator cit = this->avaliable_items.cbegin();
        cit != this->avaliable_items.cend();
        ++cit)
    {
        ss << std::distance(this->avaliable_items.cbegin(), cit) << "\t" << cit->first << " [ ";
        std::vector<director_id>::const_iterator vit = cit->second.begin();
        while(vit != cit->second.end())
        {
            ss << *vit << " ";
            vit++;
        }
        ss << "]\n";
    }
    ss << "\n";
    for (std::list<std::pair<play_name, std::vector<director_id>>>::const_iterator cit = this->busy_items.cbegin();
        cit != this->busy_items.cend();
        ++cit)
    {
        ss << std::distance(this->busy_items.cbegin(), cit) << "\t" << cit->first << " [ ";
        std::vector<director_id>::const_iterator vit = cit->second.begin();
        while(vit != cit->second.end())
        {
            ss << *vit << " ";
            vit++;
        }
        ss << "]\n";
    }
    return ss.str();
}
*/

void
threadsafe_menu::reg_play(play_name play_name_, director_id id)
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    // // check avaliable map, if not exist this play, add it
    // if (this->play_name_available_map.find(play_name_) == this->play_name_available_map.end())
    //     this->play_name_available_map[play_name_] = 1;
    // if this->memu has id, add it into id's bucket
    *safe_io << "REG" << play_name_, safe_io->flush();
    if (this->menu.find(id) != this->menu.end())
    {
        *safe_io << "ADD" << id << " " << play_name_, safe_io->flush();
        this->menu[id].second.insert(play_name_);
    // if not, set a new director pair
    } else
    {
        *safe_io << "CREATE" << id << " " << play_name_, safe_io->flush();
        std::vector<play_name> v{play_name_};
        //status_play_names_type p = std::make_pair(AVAILABLE, v);
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
/*
bool
threadsafe_menu::find_in_busy(director_id id)
{
    for (std::list<std::pair<play_name, std::vector<director_id>>>::iterator it = this->busy_items.begin();
        it != this->busy_items.end();
        ++it
    )
    {
        if (std::find(it->second.begin(), it->second.end(), id) != it->second.end())
            return true;
    }
    return false;
}


void
threadsafe_menu::reg_busy_play(play_name play_name_, director_id id)
{
    //std::lock_guard<std::mutex> lock(this->menu_mutex);
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
*/

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
    this->menu.erase(this->menu.find(id));
}

void
threadsafe_menu::empty()
{
    std::lock_guard<std::mutex> lock(this->menu_mutex);
    this->menu.clear();
    this->play_name_available_map.clear();
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
    *safe_io << "GET " << it->first, safe_io->flush();
    return it->first;
}