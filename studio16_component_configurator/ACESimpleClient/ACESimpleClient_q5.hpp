#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
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

#include "ace/OS_main.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

using namespace std;

#define SUCCESS 0
#define BUFFER_SIZE 256


class client : public ACE_Event_Handler
{
    size_t argc;
    char** argv;

    ~client()
    {
        cout << "release client" << endl;
    }

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
        cout << "handle close at " << this_thread::get_id() << endl;
        if ( (mask & TIMER_MASK) && (mask & SIGNAL_MASK) )
            cout << "handle close with TIMER_MASK and SIGNAL_MASK" << endl;
        if ( !(mask & TIMER_MASK) && (mask & SIGNAL_MASK) )
            cout << "handle close with SIGNAL_MASK" << endl;
        if ( (mask & TIMER_MASK) && !(mask & SIGNAL_MASK) )
            cout << "handle close with TIMER_MASK" << endl;
        delete this;

        return 0;
    }
    
};

class service_wrapper : public ACE_Task_Base {
    client* client_service = nullptr;

    int timer_id = -1;

    int canceled_;
    ACE_Condition<ACE_Thread_Mutex> *cancel_cond_;

    ACE_Thread_Mutex *mutex_;
public:
    ~service_wrapper()
    {
        cout << "release service" << endl;
    }
    virtual int init(int argc, ACE_TCHAR *argv[]){
        cout << "Starting up the time Service at " << this_thread::get_id() << "\n";

        client_service = new client(argc, argv);

        mutex_ = new ACE_Thread_Mutex;

        cancel_cond_ = new ACE_Condition<ACE_Thread_Mutex>(*mutex_);

        activate(THR_NEW_LWP|THR_DETACHED);

        return 0;
    }

    virtual int fini(void)
    {
        cout << "FINISH!Closing down the Time Service\n";
        canceled_=1;

        ACE_Reactor::instance()->end_reactor_event_loop();

        mutex_->acquire();

        while(canceled_)

        cancel_cond_->wait();

        mutex_->release();

        cout << "fini done" << endl;

        
        return 0;
    }

    virtual int suspend(void)
    {
        cout << "Time Service has been suspended\n";

        ACE_Reactor::instance()->cancel_timer(this->timer_id);
        ACE_Reactor::instance()->suspend_handler(this->client_service);
        
        int result=ACE_Task_Base::suspend();

        return result;
    }

    virtual int resume(void)
    {
        cout << "Resuming Time Service\n";

        ACE_Time_Value time_interval(3, 0);
        ACE_Time_Value time(0, 0);
        timer_id = ACE_Reactor::instance()->schedule_timer(this->client_service, 0, time, time_interval);

        ACE_Reactor::instance()->resume_handler(this->client_service);

        int result=ACE_Task_Base::resume();

        return result;
    }

    virtual int svc(void)
    {

        cout << "start svc at at " << this_thread::get_id() << "\n";
        singleton* single = singleton::GetInstance();
        //client client1(argc, argv);
        single->update(this, "client service");
        //client client2(argc, argv);
        //single->update(&client2, "client2");

        // ACE_SOCK_Acceptor file_acceptor;
        // if (file_acceptor.open(ACE_STDIN) < 0)
        // {
        //     cout << "Error: Can not listen on stdin" << endl;
        //     return EINVAL;
        // }
        
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
        timer_id = reactor->schedule_timer(this->client_service, 0, time, time_interval);


        // register event handler
        int ret = reactor->register_handler(SIGINT, this->client_service);

        // Then, your client program should call: wait() to wait forever while the timer runs.
        //ACE_Thread_Manager::instance()->wait();

        // Then, instead of calling ACE_Thread_Manager::instance()->wait(); call ACE_Reactor::instance()->run_reactor_event_loop(); to run the reactor's event loop (forever) while the handler is repeatedly called for timer events (that are now coming from the reactor).
        ret = reactor->run_reactor_event_loop();
        if ( ret < 0)
            cout << "Error in ACE_Reactor::instance()->run_reactor_event_loop() with error code: " << ret << endl;

        ret = ACE_Reactor::instance()->cancel_timer(this->timer_id);
        if ( ret < 0)
            cout << "Error in ACE_Reactor::instance()->cancel_timer() with error code: " << ret << endl; 

        ret = ACE_Reactor::instance()->remove_handler(this->client_service, ACE_Event_Handler::NULL_MASK);
        if ( ret < 0)
            cout << "Error in ACE_Reactor::instance()->remove_handler() with error code: " << ret << endl;
        
        //ACE_Reactor::instance()->close();
        ret = ACE_Reactor::instance()->reactor_event_loop_done();
        if ( ret < 0)
            cout << "Error in ACE_Reactor::instance()->reactor_event_loop_done() with error code: " << ret << endl;
        
        ACE_Reactor::instance()->reset_reactor_event_loop();
        cout << "end and close done" << endl;
        
        
        canceled_=0;

        cancel_cond_->signal();

        ACE_DEBUG((LM_DEBUG, "Signalled main task that Time Service is exiting \n"));

        return 0;
    }

    virtual int info(ACE_TCHAR *argv[], size_t length)
    {
        cout << "info" << endl;
        return 0;
    }

};



