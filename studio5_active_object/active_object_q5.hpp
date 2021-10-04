#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include "monitor_object_q5.hpp"

template<class T>
class active_object
{
    synchronized_list<T> sync_list_;
    std::mutex active_mutex_;
    bool active_;
    active_object<T> * other_;

    void run_service()
    {
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "THREAD " << this_id << " : run_service started." << std::endl;
        while (active_)
        {
            std::shared_ptr<T> result = sync_list_.pop_back();
            if (result != nullptr)
                std::cout <<  "run_service() get: " << *(result.get()) << std::endl;
            if (other_ != 0 && !sync_list_.empty())
            {
                other_->enqueue(*(result.get()));
            }
        }
        std::cout << "THREAD " << this_id << " : run_service stopped." << std::endl;
    }
public:
    explicit active_object(active_object<T>* other = 0) : other_(other)
    {
        active_ = true;
    }
    
    void enqueue(T new_data)
    {
        sync_list_.push_front(new_data);
    }
    void active()
    {
        if(!active_) active_ = true;
        sync_list_.active();
        std::thread local_thread(&active_object::run_service, this);
        local_thread.detach();
    }

    void deactivate()
    {   
        if (active_) sync_list_.shut_down();
        active_ = false;
    }

};