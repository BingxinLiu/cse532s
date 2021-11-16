#include <iostream>
#include <string>
#include <cstring>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"

using namespace std;

#define SUCCESS 0
#define BUFFER_SIZE 256

int 
main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Error: No command line arguments." << endl;
        return EINVAL;
    }
     
    string arg(argv[argc-1]);
    cout << arg << endl;

    ACE_TCHAR buffer[BUFFER_SIZE];

    ACE_INET_Addr address(8086, ACE_LOCALHOST);
    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream ace_sock_stream;

    if ( connector.connect(ace_sock_stream, address) < 0 )
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        cout << "Error: Can not connect to " << buffer << endl;
        return EINVAL;
    }
    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    cout << "Start connect to " << buffer << endl;

    for (size_t i = 1; i < argc; ++i)
    {
        if ( ace_sock_stream.send_n(argv[i], strlen(argv[i])) < 0 )
            cout << "Error: Can not send [" << string(argv[i]) << "]" << endl; 
    }

    ace_sock_stream.close();




    return SUCCESS;
}