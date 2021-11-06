#pragma once

#include <iostream>
#include <thread>

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

struct state_snapshot
{
    std::thread::id thread_id;
    void* this_object_ptr;
    void* current_function;
    void* additional_object;

    state_snapshot()
    {
        this->thread_id = std::this_thread::get_id();
        this->this_object_ptr = nullptr;
        this->current_function = nullptr;
        this->additional_object = nullptr;
        //std::cout << "default constructor" << std::endl;
    }

    state_snapshot( std::thread::id id, \
                    void* current_function, \
                    void* object_ptr = nullptr, \
                    void* additional_object  = nullptr\
    ) : thread_id(id), \
        this_object_ptr(object_ptr), \
        current_function(current_function), 
        additional_object(additional_object)
    {
        //std::cout << "argument constructor" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const state_snapshot& ss);
};