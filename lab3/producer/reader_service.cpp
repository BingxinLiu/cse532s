#include "reader_service.hpp"
#include "../utilities/utilities.hpp"

// get producer, ace_sock_stream
reader_service::reader_service(producer& producer_, ACE_SOCK_Stream* ss, uint id)
    : producer_(producer_), ace_sock_stream(ss), id(id)
{}

// release the socket
reader_service::~reader_service()
{
    if (DEBUG)
    {
        *(threadsafe_io::get_instance()) << "release reader_service and sock stream for reader";
        threadsafe_io::get_instance()->flush();
    }
    delete this->ace_sock_stream;
}

ACE_HANDLE 
reader_service::get_handle() const
{
    return this->ace_sock_stream->get_handle();
}

// handle messages coming from directors
// parse each message and do corresponding operations.
int
reader_service::handle_input(ACE_HANDLE h)
{
    char buffer[BUFFER_SIZE];
    size_t recv_len = 0;

    recv_len = this->ace_sock_stream->recv(&buffer, BUFFER_SIZE);
    if (recv_len <= 0)
    {
        *safe_io << "The socket is closed.", safe_io->flush();
        int ret;
        ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
        if ( ret < 0)
            *safe_io << "remove handler failed", safe_io->flush();
        *safe_io << "reader remove self", safe_io->flush();
        return EXIT_FAILURE;
    }

    std::string recv = std::string(buffer);
    
    if (DEBUG)
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
        // if ID COMMAND
        // send the id for this new director
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
                
                if (DEBUG)
                    *safe_io << "SEND [" << ss.str() << "]", safe_io->flush();
            }
        }

        // register new play of a director
        // add it into menu
        // print refresh the menu
        if (command == REG_PLAY_COMMAND)
        {
            uint id;
            std::string playname;
            if (ss >> id >> playname)
            {
                playname = trim(playname);
                this->producer_.menu.reg_play(playname, id);
                *safe_io << this->producer_.menu.str();
                safe_io->flush();
            }
        }

        // move the played play from unclear status into bust status
        if (command == STARTED_CONFIRM)
        {
            uint id;
            std::string playname;
            if (ss >> id >> playname)
            {
                playname = trim(playname);
                this->producer_.menu.set_busy_with_id(id);
                if (DEBUG)
                {
                    *safe_io << id << " " << playname << "STARTED";
                    safe_io->flush();
                }
            }
        }

        // Only after getting the stopped confirm message, can we set the play as available and refresh menu
        if (command == STOPPED_CONFIRM)
        {
            uint id;
            std::string playname;
            if (ss >> id >> playname)
            {
                playname = trim(playname);
                this->producer_.menu.set_idle_with_id(id);
                *safe_io << this->producer_.menu.str();
                safe_io->flush();
            }
        }

        // If a director quit or we send quit command, we will receive the quit confirm message, only after that can we release the resources about this director.
        // remove it from menu and map record, close the socket and refresh the menu.
        if (command == QUIT_CONFIRM)
        {
            uint id;
            if (ss >> id)
            {
                int ret;
                ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
                if ( ret < 0)
                    *safe_io << "remove handler failed", safe_io->flush();
                this->producer_.menu.clean_with_id(id);
                this->producer_.id_socket_map.erase(this->producer_.id_socket_map.find(id));
                this->ace_sock_stream->close();
                *safe_io << this->producer_.menu.str();
                safe_io->flush();
            }
        }
    }

}

// handle close
// remove self.
int
reader_service::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    if (DEBUG)
        *safe_io << "handle close in reader ", safe_io->flush();
    if ( mask & READ_MASK )
    {
        if (DEBUG)
        {
            *safe_io << "with READ_MASK\n";
            *safe_io << "try to delete this, id: " << this->id;
            safe_io->flush();
        }   
        delete this;
    }
    return EXIT_SUCCESS;
}
