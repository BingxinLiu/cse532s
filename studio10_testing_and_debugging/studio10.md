# STUDIO 10 : Testing and Debugging Studio

## Q1:

Bingxin Liu

## Q2:

```c++
// CODE:
// debugging.hpp
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

// testing_and_debugging.cpp
#include <iostream>

#include "debugging.hpp"

#define ZERO 0

using namespace std;

event debugging_event = func_be_called;

int
main(int, char * [])
{
    cout << debugging_event << endl;
    cout << (debugging_event = func_return) << endl;
    return ZERO;
}
```

```shell
[bingxin.liu@shell studio10_testing_and_debugging]$ ./testing_and_debugging
called
returning
[bingxin.liu@shell studio10_testing_and_debugging]$
```

