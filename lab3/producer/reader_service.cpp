#include "reader_service.hpp"

reader_service::reader_service(ACE_SOCK_Stream* ss)
    : ace_sock_stream(ss)
{}

reader_service::~reader_service()
{
    *(threadsafe_io::get_instance()) << "release reader_service and sock stream for reader";
    threadsafe_io::get_instance()->flush();
    delete this->ace_sock_stream;
}

ACE_HANDLE 
reader_service::get_handle() const
{
    return this->ace_sock_stream->get_handle();
}

int
reader_service::handle_input(ACE_HANDLE h)
{
    char buffer[BUFFER_SIZE];
    size_t recv_len = 0;

    // TODO: take care of the situation that length is larger than 256
    recv_len = this->ace_sock_stream->recv(&buffer, BUFFER_SIZE);
    if (recv_len <= 0)
    {
        *(threadsafe_io::get_instance()) << "The socket is closed.";
        threadsafe_io::get_instance()->flush();
        int ret;
        ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
        if ( ret < 0)
            *(threadsafe_io::get_instance()) << "remove handler failed", threadsafe_io::get_instance()->flush();
        ret = this->ace_sock_stream->close();
        if ( ret < 0)
            *(threadsafe_io::get_instance()) << "close socket failed", threadsafe_io::get_instance()->flush();
        *(threadsafe_io::get_instance()) << "reader remove self", threadsafe_io::get_instance()->flush();
        return EXIT_FAILURE;
    }
    
    *(threadsafe_io::get_instance()) << "RECV [" << std::string(buffer) << "]", threadsafe_io::get_instance()->flush();
    return EXIT_SUCCESS;

}

int
reader_service::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    *(threadsafe_io::get_instance()) << "handle close in reader ", threadsafe_io::get_instance()->flush();
    if ( mask & READ_MASK )
    {
        *(threadsafe_io::get_instance()) << "with READ_MASK\n";
        // if ( this->has_not_released )
        // {
            *(threadsafe_io::get_instance()) << "try to delete this";
            delete this;
        // }
    }
    threadsafe_io::get_instance()->flush();
    return EXIT_SUCCESS;
}
