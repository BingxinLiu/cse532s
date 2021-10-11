#include "lock_free.hpp"

#define ZERO 0

int
main (int, char * [])
{
    lock_free_stack<int> stack;
    for (int i = 0; i < 10; ++i)
    {
        stack.push(i);
    }
    return ZERO;
}
