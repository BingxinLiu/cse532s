
#include "producer.hpp"

#define BUFFER_SIZE 256

producer::producer(int port)
    : port(port)
{
    ACE_TCHAR buffer[BUFFER_SIZE];
    ACE_INET_Addr address(this->port, ACE_LOCALHOST);
    ACE_SOCK_Acceptor acceptor;

    if (acceptor.open(address, 1) < 0)
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        std::cout << "Error: Can not listen on " << buffer << std::endl;
        return;
    }
    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    std::cout << "Start listening on " << buffer << std::endl;

    this->listener_srv = new listener_service(acceptor);

    this->ui_srv = new ui_service();

    this->ui_srv->register_service();
    ACE_Reactor::instance()->register_handler(this->listener_srv, ACE_Event_Handler::ACCEPT_MASK);
}

producer::~producer()
{
    delete this->listener_srv;
    delete this->ui_srv;
}

int
producer::handle_signal(int signal, siginfo_t* sig, ucontext_t* ucontx)
{
    *(threadsafe_io::get_instance()) << "Producer handle signal", threadsafe_io::get_instance()->flush();
    int ret;
    ret = ACE_Reactor::instance()->end_event_loop();

    if (ret < 0)
    {
        *(threadsafe_io::get_instance()) << "Error in ACE_Reactor::instance()->end_reactor_event_loop() with error code: " << ret, threadsafe_io::get_instance()->flush();
    }

    ret = ACE_Reactor::instance()->close();
    if (ret < 0)
    {
        *(threadsafe_io::get_instance()) << "Error in ACE_Reactor::instance()->close() with error code: " << ret, threadsafe_io::get_instance()->flush();
    }
    *(threadsafe_io::get_instance()) << "End and close done", threadsafe_io::get_instance()->flush();

    return EXIT_SUCCESS;
}
