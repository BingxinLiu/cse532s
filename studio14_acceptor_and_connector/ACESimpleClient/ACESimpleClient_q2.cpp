#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Event_Handler.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Thread_Manager.h"

#include "ace/Timer_Heap.h"

#include "ace/Reactor.h"

using namespace std;

#define SUCCESS 0
#define BUFFER_SIZE 256

class client : public ACE_Event_Handler
{
    size_t argc;
    char** argv;
public:

    client(int argc, char* argv[]) : argc((size_t)argc), argv(argv) {}

    virtual int handle_timeout (const ACE_Time_Value& ace_time_value, const void* _ )
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

        string str = "0";
        if ( ace_sock_stream.send_n(str.c_str(), str.length()+1) < 0 )
                cout << "Error: Can not send [client_id:0]" << endl;

        char id_buffer[BUFFER_SIZE]; 
        size_t recv_len = 0;
        size_t id;
        while (true)
        {
            recv_len = ace_sock_stream.recv(&id, sizeof(size_t));
            if ( recv_len < 0 )
            {
                cout << "Error: Can not receive client id" << endl;
                return ace_sock_stream.close();
            } else if (recv_len == 0)
            {
                cout << "Empty recv" << endl;
                continue;
            } else
            {
                cout << "id is " << to_string(id) << endl;
                // try
                // {
                //     id = stoul( string(id_buffer) );
                // }
                // catch(...)
                // {
                //     cout << "Error: Can not parse client id" << string(id_buffer) << endl;
                //     return ace_sock_stream.close();
                // }
                break;
                
            }
            
        }

        for (size_t i = 1; i < argc; ++i)
        {
            stringstream ss;
            ss << "[" << id << "] " << argv[i];
            string send_str = ss.str();
            cout << "send" << send_str << endl;
            if ( ace_sock_stream.send_n(send_str.c_str(), send_str.length()) < 0 )
                cout << "Error: Can not send [" << send_str << "]" << endl; 
        }

        return ace_sock_stream.close();

    }

    virtual int handle_signal (int signal, siginfo_t* = 0, ucontext_t* = 0)
    {
        cout << "handle signal" << endl;
        ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
        ACE_Reactor::instance()->end_reactor_event_loop();
        ACE_Reactor::instance()->close();
        return 0;
    }

    virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
    {
        if ( (mask & TIMER_MASK) && (mask & SIGNAL_MASK) )
            cout << "handle close with TIMER_MASK and SIGNAL_MASK" << endl;
        if ( !(mask & TIMER_MASK) && (mask & SIGNAL_MASK) )
            cout << "handle close with SIGNAL_MASK" << endl;
        if ( (mask & TIMER_MASK) && !(mask & SIGNAL_MASK) )
            cout << "handle close with TIMER_MASK" << endl;

        return 0;
    }

};

int 
main(int argc, char* argv[])
{
    client client(argc, argv);

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


    // register event handler
    reactor->register_handler(SIGINT, &client);

    // Then, your client program should call: wait() to wait forever while the timer runs.
    //ACE_Thread_Manager::instance()->wait();

    // Then, instead of calling ACE_Thread_Manager::instance()->wait(); call ACE_Reactor::instance()->run_reactor_event_loop(); to run the reactor's event loop (forever) while the handler is repeatedly called for timer events (that are now coming from the reactor).
    reactor->run_reactor_event_loop();


    // Build your client program and run your client and server programs to confirm that they still communicate (with the client now running repeatedly). Your client should call the server and send its message about every 3 seconds

    return SUCCESS;
}