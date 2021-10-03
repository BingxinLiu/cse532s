#include "monitor_object.hpp"
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>

int
main(int, char * [])
{
    std::vector<std::thread> threads;
    synchronized_list<int> sync_list;
    for (int i = 5; i < 10; ++i)
    {
        threads.push_back(std::thread([=, &sync_list](){
            sync_list.push_front(i);
        }));
    }
        
    for (int i = 4; i >= 0; --i)
    {
        threads.push_back(std::thread([=, &sync_list](){
            sync_list.push_back(i);
        }));
    }
        
    std::for_each(threads.begin(), threads.end(),\
        std::mem_fn(&std::thread::join));
    threads.clear();
    std::cout << "pop back:" << std::endl;
    while (!sync_list.empty())
        threads.push_back(std::thread([&](){
            std::cout << *(sync_list.pop_back().get()) << "\t";
        }));
    std::for_each(threads.begin(), threads.end(),\
        std::mem_fn(&std::thread::join));
    std::cout << std::endl;
}