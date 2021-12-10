#include <iostream>
#include <string>
#include <vector>

#include "director.hpp"
#include "../utilities/const.hpp"
#include "../utilities/threadsafe_io.hpp"

enum args{NAME, PORT, IP_ADDRESS, MIN_THREADS, SCRIPT_FILES};

void print_usage(const char[]);

int
main(int argc, char* argv[])
{
    if (argc < DIRECTOR_ARGS_MIN_NUM)
    {
        print_usage(argv[args::NAME]);
        return EINVAL;
    }

    u_short port;
    std::string ip_address;
    unsigned int min_threads;
    std::vector<std::string> scripts_filename;
    director* director_ = nullptr;
    try
    {
        port = atoi(argv[args::PORT]);
        ip_address = std::string(argv[args::IP_ADDRESS]);
        min_threads = atoi(argv[args::MIN_THREADS]);
        if (min_threads < 0)
        {
            std::cout << "Error: minimum of threads number should not less than zero" << std::endl;
            print_usage(argv[args::NAME]);
        }
        for (int i = args::SCRIPT_FILES; i < argc; ++i)
        {
            scripts_filename.push_back(argv[i]);
            std::cout << "[ADD NEW SCRIPT]" << scripts_filename.back() << std::endl;
        }

        director_ = new director(port, ip_address, min_threads, scripts_filename);

    } catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EINVAL;
    }

    ACE_Reactor::instance()->register_handler(SIGINT, director_);

    director_->regis_self();

    ACE_Reactor::instance()->run_reactor_event_loop();

    delete director_;

    return SUCCESS;

    

}

void
print_usage(const char name[])
{
    std::cout << "usage: " << name << "<port> <ip_address> <min_threads> <script_file>+" << std::endl;
}