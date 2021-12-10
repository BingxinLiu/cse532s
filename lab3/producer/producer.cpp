
#include "producer.hpp"

uint producer::director_id = 1;

producer::producer(ACE_SOCK_Acceptor acceptor)
    : acceptor(acceptor)
{
    this->ui_srv = new ui_service(this);
    if (this->ui_srv->register_service() < 0)
    {
        *safe_io << "Error: UI_service register failed.";
        safe_io->flush();
    }
}

producer::~producer()
{
    *safe_io << "Release producer", safe_io->flush();
    delete this->ui_srv;
}

void
producer::send_msg(uint id, const std::string str)
{
    ACE_SOCK_Stream& ass = *(this->id_socket_map[id]);
    ass.send_n(str.c_str(), str.length() + 1);
}

void
producer::send_quit_all()
{
    std::string str("[QUIT]");
    for (std::map<uint, ACE_SOCK_Stream*>::iterator it = this->id_socket_map.begin();
    it != this->id_socket_map.end();
    ++it)
    {
        it->second->send_n(str.c_str(), str.length() + 1);
    }
}

ACE_HANDLE 
producer::get_handle() const
{
    return this->acceptor.get_handle();
}

int
producer::handle_input(ACE_HANDLE h)
{
    *safe_io << "listener handle connect", safe_io->flush();

    ACE_SOCK_Stream* ace_sock_stream = new ACE_SOCK_Stream;

    if (this->acceptor.accept(*ace_sock_stream) < 0)
    {
        *safe_io << "Error: Can not accept the ace_sock_stream", safe_io->flush();
        return EISCONN;
    }

    uint id = producer::director_id++;

    this->id_socket_map[id] = ace_sock_stream;

    reader_service* reader = new reader_service(*this, ace_sock_stream, id);
    ACE_Reactor::instance()->register_handler(reader, ACE_Event_Handler::READ_MASK);
    *safe_io << "handle connect done, hand over to reader_service.", safe_io->flush();

    return EXIT_SUCCESS;
}

int
producer::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    if ( (mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
        *safe_io << "handle close with ACCEPT_MASK and SIGNAL_MASK";
    if ( !(mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
        *safe_io << "handle close with SIGNAL_MASK";
    if ( (mask & ACCEPT_MASK) && !(mask & SIGNAL_MASK) )
        *safe_io << "handle close with ACCEPT_MASK";
    
    safe_io->flush();

    return SUCCESS;
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