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

AT first, I add an active timer object of type ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> and trigger it by activate() function.
    `ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> timer_queue;`
    `timer_queue.activate();`

Then, I construct an ACE_Time_Value object and set it as 3 seconds as the time interval
    `ACE_Time_Value time_interval(3, 0);`

After that, I initialize another ACE_Time_Value object set it by gettimeofday function and add it with time intervale, so that the start time will be 3 seconds after now.
    `ACE_Time_Value time_start(ACE_OS::gettimeofday());`
    `time_start += time_interval;`

Now, let the timer_queue to schedule the timer, which we set the start time and time interval
    `timer_queue.schedule(&client, 0, time_start, time_interval);`

Then, let client program wait forever while the timer runs.
    `ACE_Thread_Manager::instance()->wait();`

Now we enter the second part of this exercise:
First, we remove the decleration of ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>, and also we don't need to activate it.
    `// ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> timer_queue;`
    `// timer_queue.activate();`

Then, we get the singleton of ACE reactor by instance() function
    `ACE_Reactor* reactor = ACE_Reactor::instance();`
    
After that, I set a timer interval via ACE_Time_Value, which will be 3 seconds each period.
    `ACE_Time_Value time_interval(3, 0);`

We don't need set a start time cause we want it start until the first invocation of event loop
   ` //ACE_Time_Value time_start(ACE_OS::gettimeofday());`
   ` //time_start += time_interval;`
    `ACE_Time_Value time(0, 0);`

We also don't need the timer queue scheduler, reactor will help us to do that
    `//timer_queue.schedule(&client, 0, time_start, time_interval);`

Let reactor register the event handler and schedule timer with start time and time interval
    reactor->schedule_timer(&client, 0, time, time_interval);

We don't need to wait, cause reactor will do a busy loop
    `//ACE_Thread_Manager::instance()->wait();`

Let reactor do its busy loop
    `reactor->run_reactor_event_loop();`

When at some point and some place, someone calls end loop, we return from the busy loop and return.
    `return SUCCESS;`

## Q5:

Just move the logic of accepting and receiving logic in to the range of handle_input function. Add some neccessary dependent variable and construct function of the class derived from ACE_Event_Handler
The class structure is like:

```c++
class server : public ACE_Event_Handler
{
    ACE_SOCK_Stream ace_sock_stream;
    ACE_SOCK_Acceptor* acceptor;
public:
    server(ACE_SOCK_Acceptor* acceptor) : acceptor(acceptor) {}
    virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE){...}
};
```

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