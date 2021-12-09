
#include "director.hpp"

#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"

director::director(u_short port, std::string ip_address, int min_threads, std::vector<std::string> scripts_filename)
{
    ACE_TCHAR buffer[BUFFER_SIZE];

    ACE_INET_Addr address(port, ip_address.c_str());
    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream ace_sock_stream;

    if (connector.connect(ace_sock_stream, address) < 0)
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        *safe_io << "Error: Can not connect to " << buffer;
        safe_io->flush();
        return;
    }

    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    *safe_io << "Start connect to " << buffer;
    safe_io->flush();

    //debug
    std::string str = "hello from director";
    if ( ace_sock_stream.send_n(str.c_str(), str.length()+1) < 0 )
                std::cout << "Error: Can not send [client_id:0]" << std::endl;

    this->connect_srv = new connect_service(&ace_sock_stream);
}

director::~director()
{
    delete this->connect_srv;
}

int
director::handle_signal(int signal, siginfo_t* sig, ucontext_t* ucontx)
{
    *safe_io << "handle signal";
    safe_io->flush();
    return EXIT_SUCCESS;
}