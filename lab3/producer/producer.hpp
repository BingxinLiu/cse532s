#pragma once

#include <iostream>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "ace/Event_Handler.h"

#include "listener_service.hpp"
#include "ui_service.hpp"
#include "threadsafe_menu.hpp"
#include "../utilities/threadsafe_io.hpp"

/**
 * @brief 
 * 1. interact with the user (stdin or stdout) to display status and receive commands
 * 2. interact with the director object within director program processes
 * 
 */
class producer : public ACE_Event_Handler
{
    int port;
    listener_service* listener_srv = nullptr;
    ui_service* ui_srv = nullptr;
    threadsafe_menu menu;
public:
    explicit producer(int port = 8086);
    ~producer();

    virtual int handle_signal(int signal, siginfo_t* = 0, ucontext_t* = 0);
};