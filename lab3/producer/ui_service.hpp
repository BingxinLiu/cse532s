#pragma once

#include "producer.hpp"
#include "ace/Event_Handler.h"

class producer;

// a class on behalf of communicate with user
class ui_service : public ACE_Event_Handler
{
    producer* producer_ptr = nullptr;

public:
    ui_service(producer* producer_ptr);
    ~ui_service();
    
    // parse user's commands and let producer do its operations
    void parse_command(const std::string str);
    // register self as a stdin evnt handler
    int register_service();
    
    // functions as a event handler
    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);
};