#include <typeinfo>
#include "debugging.hpp"

std::ostream& operator<<(std::ostream& os, event event_)
{
    switch (event_)
    {
    case func_call_another_func: os << "calling"; break;
    case func_be_called: os << "called"; break;
    case func_return: os << "returning"; break;
    case func_call_returned: os << "returned"; break;
    case new_object_dyn_alloc_heap: os << "allocate"; break;
    case new_object_heap_dyn_allocated: os << "allocated"; break;
    case deallocate_dyn_object: os << "deallocate"; break;
    case dyn_object_deallocated: os << "deallocated"; break;
    case acquire_mutex: os << "acquire mutex"; break;
    case mutex_acquired: os << "mutex acquired"; break;
    case release_mutex: os << "release mutex"; break;
    case mutex_released: os << "mutex released"; break;
    case thread_wait_on_cond_v: os << "wait"; break;
    case thread_wake_up_on_cond_v: os << "wake"; break;
    case thread_notify_one: os << "notify one"; break;
    case thread_notify_all: os << "notify all"; break;
    case unknown_event: os << "unknown"; break;
    default:
        os << "Error: undefined behaviour\n";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const state_snapshot& ss)
{
    os << "|---------------\n";
    os << "| id: " << ss.thread_id << " \n";
    if (ss.this_object_ptr != nullptr)
        os << "| object: " << ss.this_object_ptr << " \n";
    if (ss.current_function != nullptr)
        os << "| current function: " << ss.current_function << " \n";
    if (ss.additional_object != nullptr)
        os << "| additional object: " << ss.additional_object << " \n";
    os << "|---------------" << std::endl;
    return os;
}