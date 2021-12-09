#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "ace/Event_Handler.h"

#include "ace/Reactor.h"

#include "../utilities/threadsafe_io.hpp"
#include "reader_service.hpp"
#include "../utilities/const.hpp"

class listener_service : public ACE_Event_Handler
{
    ACE_SOCK_Acceptor& acceptor;
    threadsafe_io* io = threadsafe_io::get_instance();
public:
    listener_service(ACE_SOCK_Acceptor acceptor);
    ~listener_service()
    {
        std::cout << "Release listenner service" << std::endl;
    }

    virtual ACE_HANDLE get_handle() const;
    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);

};