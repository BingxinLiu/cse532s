#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include "monitor_object.hpp"

template<class T>
class active_object
{
    synchronized_list<T> sync_list_;
    std::mutex active_mutex_;
    bool active_;

    void run_service()
    {
        while (active_)
        {
            int result = *(sync_list_.pop_back().get());
            std::cout <<  "run_service() get: " <<  result << std::endl;
        }        
    }
public:
    explicit active_object() {
        active_ = false;
    }

    ~active_object() { active_ = false; }
    
    void enqueue(T new_data)
    {
        sync_list_.push_front(new_data);
    }
    void active()
    {
        if(!sync_list_.empty()) active_ = true;
        std::thread local_thread(&active_object::run_service, this);
        local_thread.detach();
    }
    


};