#include "active_object.hpp"

#define ZERO 0

int
main (int, char * [])
{
    active_object<int> act_obj;
    act_obj.enqueue(0);
    act_obj.run_service();
    act_obj.run_service();
    act_obj.enqueue(1);
    act_obj.enqueue(2);
    act_obj.enqueue(3);
    act_obj.run_service();
    act_obj.enqueue(4);
    act_obj.enqueue(5);
    act_obj.run_service();
    act_obj.run_service();
    

    return ZERO;
}