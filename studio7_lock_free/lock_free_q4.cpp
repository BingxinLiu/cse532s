#include "lock_free_q4.hpp"
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>
#include <iterator>

#define ZERO 0

int
main (int, char * [])
{
    std::vector<int> thread1_container;
    std::vector<int> thread2_container;
    lock_free_stack<int> stack;

    std::thread t1([&thread1_container, &stack](){
        for (int i = 0; i < 10; i += 2)
            stack.push(i);

        for (int i = 0; i < 5; ++i)
            thread1_container.push_back(*(stack.pop().get()));
    });
    std::thread t2([&thread2_container, &stack](){
        for (int i = 1; i < 10; i += 2)
            stack.push(i);

        for (int i = 0; i < 5; ++i)
            thread2_container.push_back(*(stack.pop().get()));
    });
    t1.join();
    t2.join();
    std::cout << "thread1_container:" << std::endl;
    std::ostream_iterator<int> output_iterator(std::cout, "\t");
    std::copy(thread1_container.begin(), thread1_container.end(), output_iterator);
    std::cout << std::endl;
    std::cout << "thread2_container:" << std::endl;
    std::copy(thread2_container.begin(), thread2_container.end(), output_iterator);
    std::cout << std::endl;
    return ZERO;
}
