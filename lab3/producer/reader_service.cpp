#include "reader_service.hpp"

reader_service::reader_service(ACE_SOCK_Stream* ss)
    : ace_sock_stream(ss), io(threadsafe_io::get_instance())
{}

reader_service::~reader_service()
{
    this->io << "release reader_service and sock stream for reader" << std::endl;
    delete this->ace_sock_stream;
}

virtual ACE_HANDLE 
reader_service::get_handle() const
{
    this->ace_sock_stream->get_handle();
}

virtual int
reader_service::handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE)
{
    char buffer[BUFFER_SIZE];
    size_t recv_len = 0;

    // TODO: take care of the situation that length is larger than 256
    recv_len = this->ace_sock_stream->recv(&buffer, BUFFER_SIZE);
    if (recv_len <= 0)
    {
        this->io << "The socket is closed." << std::endl;
        int ret;
        ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
        if ( ret < 0)
            this->io << "remove handler failed" << std::endl;
        ret = this->ace_sock_stream->close();
        if ( ret < 0)
            this->io << "close socket failed" << std::endl;
        this->io << "reader remove self" << std::endl;
        return EXIT_FAILURE;
    }
    this->io << "RECV [" << string(buffer) << "]" << std::endl;
    return EXIT_SUCCESS;

}