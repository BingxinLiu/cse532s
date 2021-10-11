#include "lock_free_q3.hpp"

#define ZERO 0

void pop(lock_free_stack<int>* stack)
{
    std::shared_ptr<int> result_ptr = stack->pop();
    int result;
    if (result_ptr != nullptr) result = *(result_ptr.get());
    else result = -1;
    std::cout << "Pop: " << result << std::endl;
}

int
main (int, char * [])
{
    lock_free_stack<int> stack;
    stack.push(1);
    pop(&stack);
    pop(&stack);
    stack.push(3);
    stack.push(7);
    stack.push(2);
    pop(&stack);
    pop(&stack);
    return ZERO;
}
