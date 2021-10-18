#pragma once

#include <iostream>

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

std::ostream& operator<<(std::ostream& os, enum event event_)
{
    switch (event_)
    {
    case func_call_another_func: os << "calling"; break;
    case func_be_called: os << "called"; break;
    case func_return: os << "returning"; break;
    case func_call_returned: os << "returned"; break;
    
    default:
        break;
    }
}