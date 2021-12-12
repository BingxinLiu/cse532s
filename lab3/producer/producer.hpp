#pragma once

#include <iostream>
#include <map>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "ace/Event_Handler.h"

#include "reader_service.hpp"
#include "ui_service.hpp"
#include "threadsafe_menu.hpp"
#include "../utilities/threadsafe_io.hpp"

class ui_service;
class reader_service;

/**
 * @brief 
 * 1. interact with the user (stdin or stdout) to display status and receive commands
 * 2. interact with the director object within director program processes
 * 
 */
class producer : public ACE_Event_Handler
{
    static uint director_id;
    ui_service* ui_srv = nullptr;
    ACE_SOCK_Acceptor& acceptor;
public:
    std::map<uint, ACE_SOCK_Stream*> id_socket_map;
    threadsafe_menu menu;
    producer(ACE_SOCK_Acceptor acceptor);
    ~producer();

    void send_msg(uint id, const std::string str);
    void send_quit_all();
    void wait_for_quit();

    virtual ACE_HANDLE get_handle() const;
    virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
    virtual int handle_signal(int signal, siginfo_t* = 0, ucontext_t* = 0);
};