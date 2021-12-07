# STUDIO 11 : Basic Networked Concurrency Studio

## Q1:

Bingxin Liu

## Q2:

```shell
# OUTPUT:
[bingxin.liu@linuxlab004 studio12_basic_network_concurrency]$ echo $ACE_ROOT
/home/warehouse/bingxin.liu/cse532s/ACE_wrappers
[bingxin.liu@linuxlab004 studio12_basic_network_concurrency]$ echo $LD_LIBRARY_PATH
/usr/local/cuda-11.0/lib64:/opt/ibm/lsfsuite/lsf/10.1/linux2.6-glibc2.3-x86_64/lib:/project/compute/compilers/gcc-8.3.0/lib64:/usr/lib64/mpich/lib:/usr/local/cuda-11.0/lib64:/home/warehouse/bingxin.liu/cse532s/ACE_wrappers/ace:/home/warehouse/bingxin.liu/cse532s/ACE_wrappers/ace
[bingxin.liu@linuxlab004 studio12_basic_network_concurrency]$
```

## Q3:

```shell
# OUTPUT:
[bingxin.liu@linuxlab004 ACESimpleServer]$ make
g++ -Wall -std=c++11 -pthread -DTEMPLATE_HEADERS_INCLUDE_SOURCE -o ACESimpleServer ACESimpleServer.cpp
[bingxin.liu@linuxlab004 ACESimpleServer]$ ./ACESimpleServer 111
Error: Too many command line arguments.
[bingxin.liu@linuxlab004 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
[bingxin.liu@linuxlab004 ACESimpleServer]$ cd ../ACESimpleClient/
[bingxin.liu@linuxlab004 ACESimpleClient]$ ./ACESimpleClient
Error: No command line arguments.
[bingxin.liu@linuxlab004 ACESimpleClient]$ ./ACESimpleClient 111
111
[bingxin.liu@linuxlab004 ACESimpleClient]$
```

## Q4:

The reason why it is important to check the return values of those methods is those methods may failed. For example, if a program try to listen on a port less than 2000, it may not have the permission to do that cause those ports may be reserved. Therefore, check the return value may tell us something about the state of the program.

```c++
// CODE: ACESimpleServer.cpp
#include <iostream>
#include <string>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

using namespace std;

#define SUCCESS 0
#define BUFFER_SIZE 256

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
    ACE_SOCK_Stream ace_sock_stream;
    if ( acceptor.open(address, 1) < 0)
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        cout << "Error: Can not listen on " << buffer << endl;
        return EINVAL;
    }
    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    cout << "Start listening on " << buffer << endl;

    if ( acceptor.accept(ace_sock_stream) < 0 )
    {
        cout << "Error: Can not accept the ace_sock_stream" << endl;
        return EISCONN;
    }
        
    
    char char_buffer;
    size_t len = 1;
    while ( ace_sock_stream.recv(&char_buffer, len) > 0 )
    {
        cout << char_buffer << endl;
    }
    cout << "The socket is closed." << endl;

    return SUCCESS;
}
```

```shell
# OUTPUT:
[bingxin.liu@linuxlab008 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086

[bingxin.liu@linuxlab008 ACESimpleServer]$ make
g++ -L/home/warehouse/bingxin.liu/cse532s/ACE_wrappers/ace -Wall -std=c++11 -pthread -DTEMPLATE_HEADERS_INCLUDE_SOURCE -o ACESimpleServer ACESimpleServer.cpp -I/home/warehouse/bingxin.liu/cse532s/ACE_wrappers -lACE
[bingxin.liu@linuxlab008 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Error: Can not listen on 127.0.0.1:1000
[bingxin.liu@linuxlab008 ACESimpleServer]$
```

## Q5:

The motivation of ckecking return value is basically the same as the previous exercise. A failed call to system call may cause some computer source leaking. For example, if a server doesn't start is service, but a client try to connect with the server. After the trying failed, the client should be able to release the sock fd by checking connect return value.

```c++
// CODE:
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
```

```shell
# OUTPUT
[bingxin.liu@linuxlab008 ACESimpleClient]$ ./ACESimpleClient 123
123
Error: Can not connect to 127.0.0.1:8086
[bingxin.liu@linuxlab008 ACESimpleClient]$
```
## Q6:

```shell
# OUTPUT FROM CLIENT
[bingxin.liu@linuxlab008 ACESimpleClient]$ ./ACESimpleClient 123
123
Start connect to 127.0.0.1:8086
[bingxin.liu@linuxlab008 ACESimpleClient]$ ./ACESimpleClient asd89387423
asd89387423
Start connect to 127.0.0.1:8086
[bingxin.liu@linuxlab008 ACESimpleClient]$

# OUTPUT FROM SERVER
[bingxin.liu@linuxlab008 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
1
2
3
The socket is closed.
[bingxin.liu@linuxlab008 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
a
s
d
8
9
3
8
7
4
2
3
The socket is closed.
[bingxin.liu@linuxlab008 ACESimpleServer]$
```