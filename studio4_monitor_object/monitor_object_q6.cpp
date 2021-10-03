#include "monitor_object_q6.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

int
main(int, char * [])
{
    std::vector<std::thread> threads;
    synchronized_list<int> sync_list(4, 2);
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
    for (int i = 0; i < 10; ++i)
        threads.push_back(std::thread([&](){
            assert(sync_list.pop_back().get() != nullptr);
        }));
    std::for_each(threads.begin(), threads.end(),\
        std::mem_fn(&std::thread::join));
    std::cout << std::endl;
}