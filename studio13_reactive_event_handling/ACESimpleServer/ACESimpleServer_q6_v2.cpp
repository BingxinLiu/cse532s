#include <iostream>
#include <string>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "ace/Event_Handler.h"

#include "ace/Reactor.h"

using namespace std;

#define SUCCESS 0
#define BUFFER_SIZE 256

class server : public ACE_Event_Handler
{
    ACE_SOCK_Stream ace_sock_stream;
    ACE_SOCK_Acceptor* acceptor;
public:
    server(ACE_SOCK_Acceptor* acceptor) : acceptor(acceptor) {}

    virtual ACE_HANDLE get_handle() const
    {
        return acceptor->get_handle();
    }

    virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE)
    {


        if ( acceptor->accept(ace_sock_stream) < 0 )
        {
            cout << "Error: Can not accept the ace_sock_stream" << endl;
            return EISCONN;
        }
        char char_buffer;
        size_t len = 1;
        size_t recv_len = 0;
        do
        {
            recv_len = ace_sock_stream.recv(&char_buffer, len);
            if ( recv_len == 0 ) continue;
            cout << char_buffer << endl;
        } while ( recv_len > 0 );
        cout << "The socket is closed." << endl;

        return ace_sock_stream.close();

    }
};

int 
main(int argc, char* argv[])
{
    if (argc != 1)
    {
        cout << "Error: Too many command line arguments." << endl;
        return EINVAL;
    }
     
    string arg(argv[argc - 1]);
    cout << arg << endl;

    ACE_TCHAR buffer[BUFFER_SIZE];
    ACE_INET_Addr address(8086, ACE_LOCALHOST);
    ACE_SOCK_Acceptor acceptor;
    if ( acceptor.open(address, 1) < 0)
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        cout << "Error: Can not listen on " << buffer << endl;
        return EINVAL;
    }
    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    cout << "Start listening on " << buffer << endl;

    server server(&acceptor);

    ACE_Reactor::instance()->register_handler(&server, ACE_Event_Handler::ACCEPT_MASK);
        
    ACE_Reactor::instance()->run_reactor_event_loop();

    return SUCCESS;
}