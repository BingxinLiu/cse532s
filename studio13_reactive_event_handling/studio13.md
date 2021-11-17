# Reactive Event Handling Studio

## Q1:

Bingxin Liu

## Q2:

```shell
# OUTPUT FROM CLIENT
[bingxin.liu@linuxlab005 ACESimpleClient]$ ./ACESimpleClient 123asd
123asd
Start connect to 127.0.0.1:8086
[bingxin.liu@linuxlab005 ACESimpleClient]$ ./ACESimpleClient jkl843
jkl843
Start connect to 127.0.0.1:8086
[bingxin.liu@linuxlab005 ACESimpleClient]$

# OUTPUT FROM SEVRER
[bingxin.liu@linuxlab005 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
1
2
3
a
s
d
The socket is closed.
j
k
l
8
4
3
The socket is closed.
C-c
[bingxin.liu@linuxlab005 ACESimpleServer]$
```

## Q3:

I first create a client class which is public inheritance from the ACE_Event_Handler class.
Then, I move the argc and argv into the class as private members.
Assign a constructor for client class with argc and argv as its arguments
The class has a virtual function handle_timeout and I move all the logic about connect and send into the function.

```
# OUTPUT FROM CLIENT
[bingxin.liu@linuxlab005 ACESimpleClient]$ ./ACESimpleClient 123
123
Start connect to 127.0.0.1:8086
[bingxin.liu@linuxlab005 ACESimpleClient]$ ./ACESimpleClient 1312
1312
Start connect to 127.0.0.1:8086
[bingxin.liu@linuxlab005 ACESimpleClient]$

# OUTPUT FROM SERVER
[bingxin.liu@linuxlab005 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
1
2
3
The socket is closed.
1
3
1
2
The socket is closed.
```

## Q4:

AT first
// refactor your main client program by adding an active timer object of type ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> and then calling its activate method which will launch a separate (POSIX) thread to generate timeout events.
    ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> timer_queue;
    timer_queue.activate();
    
    // Construct an ACE_Time_Value object with arguments 3 and 0 (a value of three seconds), for the interval at which to have the client's event handler wake up and contact the server. 
    ACE_Time_Value time_interval(3, 0);

    // Initialize another ACE_Time_Value object (for the starting time of the first invocation of the handler) with the value returned from calling ACE_OS::getttimeofday, and then (using ACE_Time_Value::operator+=) add the interval object to it.
    ACE_Time_Value time_start(ACE_OS::gettimeofday());
    time_start += time_interval;

    // The client program should then set up repeated invocation of the client's event handler by calling the active timer object's schedule method with the address of the client's event handler, the value 0, the starting time object, and the iterval object.
    timer_queue.schedule(&client, 0, time_start, time_interval);

    // Then, your client program should call: wait() to wait forever while the timer runs.
    ACE_Thread_Manager::instance()->wait();

Then:
// refactor your main client program by adding an active timer object of type ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> and then calling its activate method which will launch a separate (POSIX) thread to generate timeout events.
    // ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> timer_queue;
    // timer_queue.activate();

    // Then modify your client code so that instead of registering for timeout events with an ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>, it instead registers the handler to receive periodic timer events from an ACE_Reactor (hint: the ACE_Reactor::instance method returns a pointer to a singleton reactor, which should be the only reactor you need to use in your client and server programs for these exercises), as follows.
    ACE_Reactor* reactor = ACE_Reactor::instance();
    
    // Construct an ACE_Time_Value object with arguments 3 and 0 (a value of three seconds), for the interval at which to have the client's event handler wake up and contact the server. 
    ACE_Time_Value time_interval(3, 0);

    // Initialize another ACE_Time_Value object (for the starting time of the first invocation of the handler) with the value returned from calling ACE_OS::getttimeofday, and then (using ACE_Time_Value::operator+=) add the interval object to it.
    //ACE_Time_Value time_start(ACE_OS::gettimeofday());
    //time_start += time_interval;

    // Instead of constructing an ACE_Time_Value object for the time of the first invocation of the handler, construct one with arguments 0 and 0, for the time to delay until the first invocation (once the reactor's event loop is started).
    ACE_Time_Value time(0, 0);


    // The client program should then set up repeated invocation of the client's event handler by calling the active timer object's schedule method with the address of the client's event handler, the value 0, the starting time object, and the iterval object.
    //timer_queue.schedule(&client, 0, time_start, time_interval);

    // Instead of calling the active timer object's schedule method, call ACE_Reactor::instance()->schedule_timer with the address of the event handler, the value 0, the delay object, and the interval object.
    reactor->schedule_timer(&client, 0, time, time_interval);

    // Then, your client program should call: wait() to wait forever while the timer runs.
    //ACE_Thread_Manager::instance()->wait();

    // Then, instead of calling ACE_Thread_Manager::instance()->wait(); call ACE_Reactor::instance()->run_reactor_event_loop(); to run the reactor's event loop (forever) while the handler is repeatedly called for timer events (that are now coming from the reactor).
    reactor->run_reactor_event_loop();


    // Build your client program and run your client and server programs to confirm that they still communicate (with the client now running repeatedly). Your client should call the server and send its message about every 3 seconds

    return SUCCESS;

## Q5:

As the answer to this exercise, please describe what changes you made to your server program to implement it in this new manner, including how you structured the event handler class and its methods.

## Q6:

```c++
// CODE FOR ACESimpleServer_q6_v2.cpp
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
```