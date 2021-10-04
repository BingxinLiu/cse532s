#include <iostream>
#include <chrono>
#include <thread>
#include "active_object_q3.hpp"

#define ZERO 0

int
main (int, char * [])
{
    active_object<int> act_obj;

    act_obj.enqueue(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    act_obj.active();
    act_obj.active();
    // act_obj.enqueue(1);
    // act_obj.enqueue(2);
    // act_obj.enqueue(3);
    // act_obj.active();
    // act_obj.enqueue(4);
    // act_obj.enqueue(5);
    // act_obj.active();
    // act_obj.active();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    

    return ZERO;
}