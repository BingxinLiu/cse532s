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

## Q3:

```c++
// CODE:

// debugging.hpp::state_snapshot
struct state_snapshot
{
    std::thread::id thread_id;
    void* this_object_ptr;
    void* current_function;
    void* additional_object;

public:
    state_snapshot()
    {
        this->thread_id = this_thread::get_id();
        this->this_object_ptr;
        this->current_function = nullptr;
        this->additional_object = nullptr;
    }
    state_snapshot(std::thread::id id, void* current_function, void* this_object_ptr = nullptr, void* additional_object = nullptr)\
        :   thread_id(id), this_object_ptr(this_object_ptr), current_function(current_function), \
            additional_object(additional_object )
    {}

    friend ostream& operator<<(ostream& os, const state_snapshot& ss);

};

// debugging.cpp::state_snapshot
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

// testing_and_debugging_q3.cpp
#include <iostream>
#include <thread>

#include "debugging.hpp"

#define ZERO 0

using namespace std;

event debugging_event = func_be_called;

int
main(int, char * [])
{
    cout << debugging_event << endl;
    state_snapshot just_be_called(this_thread::get_id(), (void*) &main);
    cout << just_be_called << endl;


    cout << (debugging_event = func_return) << endl;
    state_snapshot just_to_return(this_thread::get_id(), (void*) &main);
    cout << just_to_return << endl;
    return ZERO;
}
```

```shell
#OUTPUT
[bingxin.liu@linuxlab005 studio10_testing_and_debugging]$ ./testing_and_debugging_q3
called
|---------------
| id: 1
| current function: 0x4008a2
|---------------

returning
|---------------
| id: 1
| current function: 0x4008a2
|---------------

[bingxin.liu@linuxlab005 studio10_testing_and_debugging]$
```

## Q4:

```c++
// CODE:
// debugging.hpp::Event_logger
class Event_logger
{
    ostream& os;
    map<void*, const string> ptr_str_map;
    mutex print_lock;

public:
    
    Event_logger();
    Event_logger(ostream& os);

    void register_(void* ptr, const string& str);
    void unregister_(void* ptr);

    Event_logger& operator<<(const state_snapshot ss);
};

// debugging.cpp::Event_logger
Event_logger::Event_logger() : os(cout) {}
Event_logger::Event_logger(ostream& os) : os(os) {}

void
Event_logger::register_(void* ptr, const string& str)
{
    (this->ptr_str_map).insert(make_pair(ptr, str));
}

void
Event_logger::unregister_(void* ptr)
{
    map<void*, const string>::iterator it = (this->ptr_str_map).find(ptr);
    if ( it != this->ptr_str_map.end() )
        this->ptr_str_map.erase(it);
}

Event_logger& 
Event_logger::operator<<(const state_snapshot ss)
{
    lock_guard<mutex> lock(print_lock);
    this->os << ss;
    map<void*, const string>::iterator it;
    it = this->ptr_str_map.find(ss.this_object_ptr);
    if ( it != this->ptr_str_map.end() )
        this->os << "| this object info:" << (*it).second << "\n";
    it = this->ptr_str_map.find(ss.current_function);
    if ( it != this->ptr_str_map.end() )
            this->os << "| current function info:" << (*it).second << "\n";
    it = this->ptr_str_map.find(ss.additional_object);
    if ( it != this->ptr_str_map.end() )
        this->os << "| additional object info:" << (*it).second << "\n";
    this->os << "|---------------" << std::endl;
}

// testing_and_debugging_q4.cpp
Event_logger event_logger;

event debugging_event = func_be_called;

int
main(int, char * [])
{

    cout << debugging_event << endl;
    state_snapshot just_be_called(this_thread::get_id(), (void*) &main);
    event_logger.register_((void*) &main, "main function");
    event_logger << just_be_called;


    cout << (debugging_event = func_return) << endl;
    state_snapshot just_to_return(this_thread::get_id(), (void*) &main);
    event_logger << just_to_return;
    return ZERO;
}
```

```shell
called
|---------------
| id: 1
| current function: 0x402ade
|---------------
| current function info:main function
|---------------
returning
|---------------
| id: 1
| current function: 0x402ade
|---------------
| current function info:main function
|---------------
```


## Q5:

```shell
#OUTPUT
[bingxin.liu@linuxlab005 studio10_testing_and_debugging]$ ./testing_and_debugging_q5
|---------------
| called
| id: 1
| current function: 0x401561
|---------------
| current function info:main function
|---------------
|---------------
| called
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| called
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| called
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| called
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| returning
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| returning
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| returning
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| returning
| id: 1
| current function: 0x401402
|---------------
| current function info:factorial function
|---------------
|---------------
| returning
| id: 1
| current function: 0x401561
|---------------
| current function info:main function
|---------------
[1636930571515602923] [1] called
[1636930571515715582] [1] called
[1636930571515806591] [1] called
[1636930571515920918] [1] called
[1636930571516022365] [1] called
[1636930571516131374] [1] returning
[1636930571516216649] [1] returning
[1636930571516298355] [1] returning
[1636930571516379945] [1] returning
[1636930571516461147] [1] returning
[bingxin.liu@linuxlab005 studio10_testing_and_debugging]$
```