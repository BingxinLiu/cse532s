#include <iostream>
#include <chrono>
#include <thread>
#include "active_object_q5.hpp"

#define ZERO 0

int
main (int, char * [])
{
    active_object<int> act_obj;
    active_object<int> second_act_obj(&act_obj);

    for (int i = 0; i < 2; ++i)
    {
        second_act_obj.active();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        second_act_obj.enqueue(0);
        second_act_obj.enqueue(1);
        second_act_obj.enqueue(2);
        second_act_obj.enqueue(3);
        second_act_obj.enqueue(4);
        second_act_obj.enqueue(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        second_act_obj.deactivate();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    

    return ZERO;
}