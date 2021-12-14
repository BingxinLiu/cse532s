#pragma once

#include "producer.hpp"
#include "../utilities/threadsafe_io.hpp"
#include "../utilities/const.hpp"

#include "ace/SOCK_Stream.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

class producer;

// A class has the reponsibility to communicate with a specifc director
// send message and receive message
// parse the received message and let producer do its operations
class reader_service : public ACE_Event_Handler
{
    // producer reference
    producer& producer_;

    // it socket communicating with its corresponding director
    ACE_SOCK_Stream* ace_sock_stream;

    // the reader_service's id, also the director's id
    uint id;
    
public:

    // cleared flag
    bool cleared = false;
    reader_service(producer& producer_, ACE_SOCK_Stream* ss, uint id);
    ~reader_service();

    // functions as a event handler
    virtual ACE_HANDLE get_handle() const;
    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
    
    // parse received message
    void parse_receive_msg(std::string msg);
};
