#pragma once

#include "ace/Event_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Reactor.h"

#include "../utilities/const.hpp"

class connect_service : public ACE_Event_Handler
{
    ACE_SOCK_Stream* ace_sock_stream;
public:
    connect_service(ACE_SOCK_Stream* ss);
    ~connect_service();

    virtual ACE_HANDLE get_handle() const;
    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
};