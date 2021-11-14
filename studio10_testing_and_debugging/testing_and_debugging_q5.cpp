#include <iostream>
#include <thread>
#include <string>

#include "debugging.hpp"

#define ZERO 0

using namespace std;

Event_logger event_logger;

event debugging_event = func_be_called;

unsigned int 
factorial(unsigned int n)
{
    debugging_event = func_be_called;
    state_snapshot just_be_called(this_thread::get_id(), (void*) &factorial, debugging_event);
    event_logger << just_be_called;
    event_logger.record(this_thread::get_id(), debugging_event);

    debugging_event = func_return;
    state_snapshot ss(this_thread::get_id(), (void*) &factorial, debugging_event);
    if ( n == 0 || n == 1) 
    {
        
        event_logger << ss;
        event_logger.record(this_thread::get_id(), debugging_event);
        return 1;
    }

    unsigned int result = n * factorial( n - 1 );
    event_logger << ss;
    event_logger.record(this_thread::get_id(), debugging_event);
    return result;
}

int
main(int, char * [])
{
    state_snapshot just_be_called(this_thread::get_id(), (void*) &main, debugging_event);
    event_logger.register_((void*) &main, "main function");
    event_logger.register_((void*) &factorial, "factorial function");
    event_logger.register_(this_thread::get_id());

    event_logger << just_be_called;
    event_logger.record(this_thread::get_id(), debugging_event);


    unsigned int _ = factorial(4);

    debugging_event = func_return;
    state_snapshot just_to_return(this_thread::get_id(), (void*) &main, debugging_event);
    event_logger << just_to_return;
    event_logger.record(this_thread::get_id(), debugging_event);

    event_logger.print();
    return ZERO;
}