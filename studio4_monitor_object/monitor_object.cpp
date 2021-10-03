#include "monitor_object.hpp"

int
main(int, char * [])
{
    synchronized_list<int> sync_list;
    for (int i = 5; i < 10; ++i)
        sync_list.push_front(i);
    for (int i = 4; i >= 0; --i)
        sync_list.push_back(i);

    std::cout << "pop front:" << std::endl;
    while(int * pop_data = sync_list.pop_front().get())
        std::cout << *pop_data << "\t";
    std::cout << std::endl;
}