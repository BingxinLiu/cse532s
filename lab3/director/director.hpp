#pragma once

#include <string>
#include <vector>

#include "ace/Event_Handler.h"

#include "connect_service.hpp"

class director : public ACE_Event_Handler
{
    connect_service* connect_srv = nullptr;
public:
    director(u_short port, std::string ip_address, int min_threads, std::vector<std::string> scripts_filename);
    ~director();

    virtual int handle_signal(int signal, siginfo_t* = 0, ucontext_t* = 0);
};