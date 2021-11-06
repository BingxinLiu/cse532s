#include <iostream>
#include "debugging.hpp"
#include <thread>
#define ZERO 0

using namespace std;

event debugging_event = func_be_called;

int
main(int, char * [])
{
    cout << debugging_event << endl;
    state_snapshot just_be_called(this_thread::get_id(), (void*) &main);
    cout << just_be_called << endl;


    cout << (debugging_event = func_return) << endl;
    state_snapshot just_to_return(this_thread::get_id(), (void*) &main);
    cout << just_to_return << endl;
    return ZERO;
}