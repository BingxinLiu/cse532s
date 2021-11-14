#pragma once

#include <iostream>
#include <thread>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

enum event
{
    func_call_another_func,
    func_be_called,
    func_return,
    func_call_returned,
    new_object_dyn_alloc_heap,
    new_object_heap_dyn_allocated,
    deallocate_dyn_object,
    dyn_object_deallocated,
    acquire_mutex,
    mutex_acquired,
    release_mutex,
    mutex_released,
    thread_wait_on_cond_v,
    thread_wake_up_on_cond_v,
    thread_notify_one,
    thread_notify_all,
    unknown_event
};

std::ostream& operator<<(std::ostream& os, event event_);

struct state_snapshot
{
    std::thread::id thread_id;
    void* this_object_ptr;
    void* current_function;
    void* additional_object;
    enum event event;

public:
    state_snapshot()
    {
        this->thread_id = this_thread::get_id();
        this->this_object_ptr;
        this->current_function = nullptr;
        this->additional_object = nullptr;
        this->event = unknown_event;
    }
    state_snapshot(std::thread::id id, void* current_function, \
                    enum event event = unknown_event, \
                    void* this_object_ptr = nullptr, \
                    void* additional_object = nullptr)
        :   thread_id(id), this_object_ptr(this_object_ptr), \
            current_function(current_function), \
            additional_object(additional_object ), \
            event(event)
    {}

    friend ostream& operator<<(ostream& os, const state_snapshot& ss);

};

class Event_logger
{
    ostream& os;
    map<void*, const string> ptr_str_map;
    map<thread::id, size_t> id_event_count_pool;
    map<thread::id, vector<pair<time_t, event>>> id_event_pool;
    map<thread::id, size_t> id_buffer_size_pool;
    mutex print_lock;
    mutex data_lock;

public:
    
    Event_logger();
    Event_logger(ostream& os);

    void register_(void* ptr, const string& str);
    void register_(thread::id id, size_t event_buffer_size = 1024);
    void unregister_(void* ptr);

    void reset(thread::id id, size_t event_buffer_size);
    void record(thread::id id, event event);
    
    void print();

    Event_logger& operator<<(const state_snapshot ss);
};