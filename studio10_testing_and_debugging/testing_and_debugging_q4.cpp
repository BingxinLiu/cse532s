#include <iostream>
#include <thread>
#include <string>

#include "debugging.hpp"

#define ZERO 0

using namespace std;

Event_logger event_logger;

event debugging_event = func_be_called;

int
main(int, char * [])
{

    cout << debugging_event << endl;
    state_snapshot just_be_called(this_thread::get_id(), (void*) &main);
    event_logger.register_((void*) &main, "main function");
    event_logger << just_be_called;


    cout << (debugging_event = func_return) << endl;
    state_snapshot just_to_return(this_thread::get_id(), (void*) &main);
    event_logger << just_to_return;
    return ZERO;
}