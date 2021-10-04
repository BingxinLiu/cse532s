#include <iostream>
#include "monitor_object.hpp"

template<class T>
class active_object
{
    synchronized_list<T> sync_list_;
public:
    active_object() {}
    void enqueue(T new_data)
    {
        sync_list_.push_front(new_data);
    }
    void run_service()
    {
        int result = *(sync_list_.pop_back().get());
        std::cout <<  "run_service() get: " <<  result << std::endl;
    }

};