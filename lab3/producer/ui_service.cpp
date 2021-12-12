#include "ui_service.hpp"

#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#include "../utilities/const.hpp"
#include "../utilities/threadsafe_io.hpp"

ui_service::ui_service(producer* producer_ptr) : producer_ptr(producer_ptr) {}

ui_service::~ui_service() {}

int
ui_service::register_service()
{
    if (ACE_Event_Handler::register_stdin_handler(this, ACE_Reactor::instance(), ACE_Thread_Manager::instance()) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void
ui_service::parse_command(const std::string str)
{
    std::stringstream ss(str);
    std::string command;

    if (ss >> command)
    {
        if (command == START_COMMAND)
        {
            uint offset;
            if (ss >> offset)
            {
                *safe_io << command << " " << offset, safe_io->flush();
                std::string playname = this->producer_ptr->menu[offset];
                if (playname.size() == 0) return;
                uint id = this->producer_ptr->menu.pop_avaliable(playname);
                if (id == 0)
                {
                    *safe_io << "SORRY, NOT AVAILABLE", safe_io->flush();
                    return;
                }
                *safe_io << this->producer_ptr->menu.str();
                safe_io->flush();
                std::stringstream send_ss;
                send_ss << START_COMMAND << " " << playname;
                this->producer_ptr->send_msg(id, send_ss.str());
                *safe_io << "SEND [" << send_ss.str() << "]";
                safe_io->flush();
                
            }
            return;
        }
        if (command == STOP_COMMAND)
        {
            uint offset;
            if (ss >> offset)
            {
                *safe_io << command << " " << offset, safe_io->flush();
                std::string playname = this->producer_ptr->menu[offset];
                if (playname.size() == 0) return;
                uint id = this->producer_ptr->menu.pop_busy_play(playname);
                if (id == 0)
                {
                    *safe_io << "SORRY, NOT AVAILABLE", safe_io->flush();
                    return;
                }

                std::stringstream send_ss;
                send_ss << STOP_COMMAND << " " << playname;
                this->producer_ptr->send_msg(id, send_ss.str());
                *safe_io << "SEND [" << send_ss.str() << "]";
                safe_io->flush();

            }
            return;
        }
        if (command == QUIT_COMMAND)
        {
            std::string str;
            if (ss >> str && str.length() > 0)
            {
                *safe_io << "WARNING: Remaining command: " << str, safe_io->flush();
            }
            else
            {
                this->producer_ptr->send_quit_all();
                *safe_io << "QUIT", safe_io->flush();
                this->producer_ptr->handle_close(ACE_INVALID_HANDLE, SIGNAL_MASK);
                this->producer_ptr->wait_for_quit();   
            }
            return;
                

        }

    }
    *safe_io << "ERROR: Cannot parse command: " << ss.str(), safe_io->flush();
}

int
ui_service::handle_input(ACE_HANDLE h)
{
    if (h == ACE_STDIN)
    {
        char buffer[BUFFER_SIZE];
        size_t recv_len = ACE_OS::read(h, buffer, BUFFER_SIZE);
        if (recv_len <= 0) return FAILURE;
        // remove new line character
        *(threadsafe_io::get_instance()) << "UI RECV [" << std::string(buffer).substr(BEGINNING, std::string(buffer).length() - 1) << "]";
        threadsafe_io::get_instance()->flush();

        this->parse_command(std::string(buffer));

        memset(buffer, 0, BUFFER_SIZE);

        return SUCCESS;
    }
    *(threadsafe_io::get_instance()) << "WARNING: UI_SERVICE handle input, but not from stdin.";
    threadsafe_io::get_instance()->flush();
    return SUCCESS;
}

int
ui_service::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    *(threadsafe_io::get_instance()) << "Closing UI_SERVICE...";
    threadsafe_io::get_instance()->flush();
    delete this;
    return SUCCESS;
}