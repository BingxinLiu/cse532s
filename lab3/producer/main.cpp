#include <iostream>

#include "producer.hpp"

#define ENV_ARG_SIZE 2
#define PORT_OFFSET 1
#define SUCCESS 0
#define BUFFER_SIZE 256
/**
 * @brief 
 * initialize a producer object
 * register appropriate event handlers
 * run a reactive event loop to handle any signals, exceptions, or other events that may occur
 * @return int 
 */
int
main(int argc, char* argv[])
{
    if (argc > ENV_ARG_SIZE)
    {
        std::cout << "Error: Too many command line arguments." << std::endl;
        std::cout << "usage: " << argv[0] << "[port]" << std::endl;
        return EINVAL;
    }

    int port;
    if (argc == ENV_ARG_SIZE)
    {
        try
        {
            port = atoi(argv[PORT_OFFSET]);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return EINVAL;
        }
        if (port < 1024 || port > 65535)
        {
            std::cerr << "Error: Invalid port number" << std::endl;
            return EINVAL;
        }
    } else
    {
        port = 8086;
    }

    producer producer(port);
    
}