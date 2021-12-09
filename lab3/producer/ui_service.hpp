#pragma once

#include "ace/Event_Handler.h"

class ui_service : public ACE_Event_Handler
{

public:
    ui_service();
    ~ui_service();
    
    void parse_command(const std::string str);
    int register_service();
    

    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);
};