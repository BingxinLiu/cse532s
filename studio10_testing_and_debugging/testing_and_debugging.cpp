#include <iostream>

#include "debugging.hpp"

#define ZERO 0

using namespace std;

event debugging_event = func_be_called;

int
main(int, char * [])
{
    cout << debugging_event << endl;
    cout << (debugging_event = func_return) << endl;
    return ZERO;
}