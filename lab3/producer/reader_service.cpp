#include "reader_service.hpp"

reader_service::reader_service(producer& producer_, ACE_SOCK_Stream* ss, uint id)
    : producer_(producer_), ace_sock_stream(ss), id(id)
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
        *safe_io << "The socket is closed.", safe_io->flush();
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

    std::string recv = std::string(buffer);
    
    *safe_io << "RECV [" << recv << "]", safe_io->flush();

    this->parse_receive_msg(recv);

    return EXIT_SUCCESS;

}

void
reader_service::parse_receive_msg(std::string msg)
{
    std::stringstream ss(msg);
    std::string command;
    if (ss >> command)
    {
        if (command == ID_COMMAND)
        {
            uint id;
            if (ss >> id && id == 0)
            {
                size_t send_len = 0;
                std::stringstream ss;
                ss << ID_COMMAND << " " << this->id;
                send_len = this->ace_sock_stream->send_n(ss.str().c_str(), ss.str().length() + 1);
                if (send_len < 0)
                    *safe_io << "Error: Can not send " << ss.str(), safe_io->flush();
                *safe_io << "SEND [" << ss.str() << "]", safe_io->flush();
            }
        }
        if (command == REG_PLAY_COMMAND)
        {
            uint id;
            std::string playname;
            if (ss >> id >> playname)
            {
                this->producer_.menu.reg_play(playname, id);
                *safe_io << this->producer_.menu.str();
                safe_io->flush();
            }
        }
    }

}

int
reader_service::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
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
