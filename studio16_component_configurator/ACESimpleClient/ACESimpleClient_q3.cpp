#include "ACESimpleClient_q3.hpp"

using namespace std;

#define SUCCESS 0

int 
main(int argc, char* argv[])
{
    singleton* single = singleton::GetInstance();
    client client1(argc, argv);
    single->update(&client1, "client1");
    client client2(argc, argv);
    single->update(&client2, "client2");

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
    reactor->schedule_timer(&client1, 0, time, time_interval);


    // register event handler
    reactor->register_handler(SIGINT, &client1);

    // Then, your client program should call: wait() to wait forever while the timer runs.
    //ACE_Thread_Manager::instance()->wait();

    // Then, instead of calling ACE_Thread_Manager::instance()->wait(); call ACE_Reactor::instance()->run_reactor_event_loop(); to run the reactor's event loop (forever) while the handler is repeatedly called for timer events (that are now coming from the reactor).
    reactor->run_reactor_event_loop();


    // Build your client program and run your client and server programs to confirm that they still communicate (with the client now running repeatedly). Your client should call the server and send its message about every 3 seconds

    // string command_line;
    // string command;
    // string address;
    // string label;
    // while (true)
    // {
    //     cin.getline(command_line);
    //     stringstream ss(command_line);
    //     if (ss >> command >> address >> label)
    //     {
    //         if (command == "rename")
    //         {

    //             singleton::GetInstance()->update(stol(address, nullptr, 16), label);

    //         }
    //     }


    // }


    return SUCCESS;
}