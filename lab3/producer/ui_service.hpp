#pragma once

#include "producer.hpp"
#include "ace/Event_Handler.h"

class producer;

class ui_service : public ACE_Event_Handler
{
    producer* producer_ptr = nullptr;

public:
    ui_service(producer* producer_ptr);
    ~ui_service();
    
    void parse_command(const std::string str);
    int register_service();
    

    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);
};