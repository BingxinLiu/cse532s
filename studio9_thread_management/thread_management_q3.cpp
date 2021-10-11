#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <atomic>
#include <vector>
#include <functional>
#include <chrono>
#include "thread_management_q4.hpp"

#define ZERO 0

std::mutex future_mutex;

unsigned int
thread_interrupted(unsigned int n)
{
    std::cout << "thread interrupted!" << std::endl;
    return n;
}


unsigned int
fib(unsigned int n)
{
    if ( n == 1 ) return 1;
    if ( n == 0 ) return 0;
    unsigned int n1 = fib(n - 1);
    unsigned int n2 = fib(n - 2);
    if (this_thread_interrupt_flag.is_set())
    {
        std::cout << "flag is set" << std::endl;
        throw thread_interrupted(n1 + n2);
    }
    return n1 + n2;
}

thread_local interrupt_stop_flag this_thread_stop_flag;
thread_local interrupt_input_flag this_thread_input_flag;
thread_local interrupt_output_flag this_thread_output_flag;
void
event_loop()
{
    while (!this_thread_stop_flag.is_set())
    {
        stop_interruption_point();
        input_interruptible_wait();
        ouput_interruptible_wait();
    }
}


int
main (int, char * [])
{

    // std::vector<interruptible_thread* > threads;
    // std::vector<std::future<unsigned int> > futures;

    // for (int i = 0; i < 10; ++i)
    // {
    //     srand((unsigned) time(NULL));
    //     std::packaged_task<unsigned int(unsigned int)>task(fib);
    //     futures.push_back(task.get_future());

    //     interruptible_thread t(std::move(task));
    //     threads.push_back(&t);

        
    // }

    // while (!this_thread_interrupt_flag.is_set())
    // {
    //     for (int i = 0; i < 10; ++i)
    //     {
    //         std::lock_guard<std::mutex> lk(future_mutex);
    //         if (futures[i].wait_for(std::chrono::milliseconds(1))
    //             == std::future_status::ready)
    //         {
    //             this_thread_interrupt_flag.set();
    //             break;
    //         }
                
    //     }
    // }

    return ZERO;

}