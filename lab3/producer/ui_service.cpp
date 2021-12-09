#include "ui_service.hpp"

#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#include "../utilities/const.hpp"
#include "../utilities/threadsafe_io.hpp"

ui_service::ui_service() {}

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