#pragma once

#include <iostream>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "ace/Event_Handler.h"

#include "listener_service.hpp"
#include "threadsafe_menu.hpp"
#include "../utilities/threadsafe_io.hpp"

class ui
{};


/**
 * @brief 
 * 1. interact with the user (stdin or stdout) to display status and receive commands
 * 2. interact with the director object within director program processes
 * 
 */
class producer
{
    int port;
    listener_service ls;
    threadsafe_menu menu;
public:
    producer(int port);
    ~producer();
};