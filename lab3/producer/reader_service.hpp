#pragma once

#include "producer.hpp"
#include "../utilities/threadsafe_io.hpp"
#include "../utilities/const.hpp"

#include "ace/SOCK_Stream.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

class producer;

class reader_service : public ACE_Event_Handler
{
    producer& producer_;
    ACE_SOCK_Stream* ace_sock_stream;
    uint id;
    
public:
    bool cleared = false;
    reader_service(producer& producer_, ACE_SOCK_Stream* ss, uint id);
    ~reader_service();

    virtual ACE_HANDLE get_handle() const;
    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
    void parse_receive_msg(std::string msg);
};
