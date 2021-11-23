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

#include "ace/OS.h"
#include "ace/Task.h"
#include "ace/Synch.h"

#include "singleton_ds.hpp"

using namespace std;

#define SUCCESS 0
#define BUFFER_SIZE 256

class client : public ACE_Task_Base
{
    size_t argc;
    char** argv;

    int canceled_;
    ACE_Condition<ACE_Thread_Mutex> *cancel_cond_;

ACE_Thread_Mutex *mutex_;
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

    virtual int init(int argc, ACE_TCHAR *argv[]){return 0;}

    virtual int fini(void){return 0;}

    virtual int suspend(void){return 0;}

    virtual int resume(void){return 0;}

    virtual int svc(void){return 0;}

};

