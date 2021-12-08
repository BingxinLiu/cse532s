
#include "producer.hpp"

#define BUFFER_SIZE 256

producer::producer(int port = 8086)
    : port(port)
{
    ACE_TCHAR buffer[BUFFER_SIZE];
    ACE_INET_Addr address(this->port, ACE_LOCALHOST);
    ACE_SOCK_Acceptor acceptor;

    if (acceptor.open(address, 1) < 0)
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        std::cout << "Error: Can not listen on " << buffer << std::endl;
        return EINVAL;
    }
    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    std::cout << "Start listening on " << buffer << std::endl;

    listener_service listener(acceptor);
    this->ls = &listener;

    ACE_Reactor::instance()->register_handler(&listener, ACE_Event_Handler::ACCEPT_MASK);
    ACE_Reactor::instance()->register_handler(SIGINT, &listener);

    ACE_Reactor::instance()->run_reactor_event_loop();
}
