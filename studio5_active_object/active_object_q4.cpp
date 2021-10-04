#include <iostream>
#include <chrono>
#include <thread>
#include "active_object_q4.hpp"

#define ZERO 0

int
main (int, char * [])
{
    active_object<int> act_obj;

    for (int i = 0; i < 2; ++i)
    {
        act_obj.active();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        act_obj.enqueue(0);
        act_obj.enqueue(1);
        act_obj.enqueue(2);
        act_obj.enqueue(3);
        act_obj.enqueue(4);
        act_obj.enqueue(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        act_obj.deactivate();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    

    return ZERO;
}