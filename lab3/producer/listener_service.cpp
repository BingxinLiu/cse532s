
#include "listener_service.hpp"

listener_service::listener_service(ACE_SOCK_Acceptor acceptor)
    : acceptor(acceptor)
{
    this->io = threadsafe_io::get_instance();
}

virtual ACE_HANDLE 
listener_service::get_handle() const
{
    return this->acceptor.get_handle();
}

virtual int
listener_service::handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE)
{
    this->io << "listener handle connect";

    ACE_SOCK_Stream* ace_sock_stream = new ACE_SOCK_Stream;
    
    if (this->acceptor.accept(*ace_sock_stream) < 0)
    {
        this->io << "Error: Can not accept the ace_sock_stream";
        return EISCONN;
    }

    reader_service* reader = new reader_service(ace_sock_stream);
    ACE_Reactor::instance()->register_handler(reader, ACE_Event_Handler::READ_MASK);
    this->io << "handle connect done, hand over to reader_service."

    return EXIT_SUCCESS;
}

virtual int
listener_service::handle_signal(int signal, siginfo_t* = 0, ucontext_t* = 0)
{
    this->io << "handle signal";
    int ret;
    ret = ACE_Reactor::instance()->end_event_loop();

    if (ret < 0)
    {
        this->io << "Error in ACE_Reactor::instance()->end_reactor_event_loop() with error code: " << ret << std::endl;
    }

    ret = ACE_Reactor::instance()->close();
    if (ret < 0)
    {
        this->io << "Error in ACE_Reactor::instance()->close() with error code: " << ret << std::endl;
    }
    this->io << "End and close done" << std::endl;

    return EXIT_SUCCESS;
}

virtual int
listener_service::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    if ( (mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
        this->io << "handle close with ACCEPT_MASK and SIGNAL_MASK" << std::endl;
    if ( !(mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
        this->io << "handle close with SIGNAL_MASK" << std::endl;
    if ( (mask & ACCEPT_MASK) && !(mask & SIGNAL_MASK) )
        this->io << "handle close with ACCEPT_MASK" << std::endl;

    return 0;
}







