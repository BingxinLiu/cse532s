#include "connect_service.hpp"

connect_service::connect_service(ACE_SOCK_Stream* ss)
    : ace_sock_stream(ss)
{}

connect_service::~connect_service()
{
    *safe_io << "release reader_service and sock stream for reader";
    safe_io->flush();
    delete this->ace_sock_stream;
}

ACE_HANDLE 
connect_service::get_handle() const
{
    return this->ace_sock_stream->get_handle();
}

int
connect_service::handle_input(ACE_HANDLE h)
{
    *safe_io << "connect service handle input", safe_io->flush();
    char buffer[BUFFER_SIZE];
    size_t recv_len = 0;

    // TODO: take care of the situation that length is larger than 256
    recv_len = this->ace_sock_stream->recv(&buffer, BUFFER_SIZE);
    if (recv_len <= 0)
    {
        *safe_io << "The socket is closed.";
        safe_io->flush();
        int ret;
        ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
        if ( ret < 0)
        *safe_io << "remove handler failed", safe_io->flush();
        ret = this->ace_sock_stream->close();
        if ( ret < 0)
            *safe_io << "close socket failed", safe_io->flush();
        *safe_io << "reader remove self", safe_io->flush();
        return EXIT_FAILURE;
    }
    
    *safe_io << "RECV [" << std::string(buffer) << "]", safe_io->flush();

    this->ace_sock_stream->close();
    return EXIT_SUCCESS;

}

int
connect_service::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    *safe_io << "handle close in reader ", safe_io->flush();
    if ( mask & READ_MASK )
    {
        *safe_io << "with READ_MASK\n";
        // if ( this->has_not_released )
        // {
            *safe_io << "try to delete this";
            delete this;
        // }
    }
    safe_io->flush();
    return EXIT_SUCCESS;
}