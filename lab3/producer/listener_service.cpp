
#include "listener_service.hpp"

listener_service::listener_service(ACE_SOCK_Acceptor acceptor)
    : acceptor(acceptor)
{}

// listener_service::~listener_service()
// {}

ACE_HANDLE 
listener_service::get_handle() const
{
    return this->acceptor.get_handle();
}

int
listener_service::handle_input(ACE_HANDLE h)
{
    *(threadsafe_io::get_instance()) << "listener handle connect", threadsafe_io::get_instance()->flush();

    ACE_SOCK_Stream* ace_sock_stream = new ACE_SOCK_Stream;
    
    if (this->acceptor.accept(*ace_sock_stream) < 0)
    {
        *(threadsafe_io::get_instance()) << "Error: Can not accept the ace_sock_stream", threadsafe_io::get_instance()->flush();
        return EISCONN;
    }

    reader_service* reader = new reader_service(ace_sock_stream);
    ACE_Reactor::instance()->register_handler(reader, ACE_Event_Handler::READ_MASK);
    *(threadsafe_io::get_instance()) << "handle connect done, hand over to reader_service.", threadsafe_io::get_instance()->flush();

    return EXIT_SUCCESS;
}

int
listener_service::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    if ( (mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
        *(threadsafe_io::get_instance()) << "handle close with ACCEPT_MASK and SIGNAL_MASK";
    if ( !(mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
        *(threadsafe_io::get_instance()) << "handle close with SIGNAL_MASK";
    if ( (mask & ACCEPT_MASK) && !(mask & SIGNAL_MASK) )
        *(threadsafe_io::get_instance()) << "handle close with ACCEPT_MASK";
    
    threadsafe_io::get_instance()->flush();

    return 0;
}







