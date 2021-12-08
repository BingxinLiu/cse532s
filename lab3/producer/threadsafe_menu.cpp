#include <sstream>
#include "threadsafe_menu.hpp"

const std::string
threadsafe_menu::str()
{
    std::stringstream ss;
    std::lock_guard<std::mutex> lk(this->menu_mutex);
    
    ss << "Name\t\tStatus\n";
    for (std::map<play_name,director_status_pair>::const_iterator cit = this->items.cbegin();
        cit != this->items.cend();
        ++cit)
    {
        ss << std::distance(this->items.cend(), cit) << ": " << cit->second.first << " " << (cit->second.second? "Ready" : "Unavailable") << "\n";
    }

    return ss.str();
}