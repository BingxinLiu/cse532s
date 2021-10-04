#include <atomic>
#include <thread>
#include <iostream>

#define ZERO 0

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true,std::memory_order_relaxed);
    y.store(true,std::memory_order_release);
}

void read_y_then_x()
{
    while(!y.load(std::memory_order_acquire));
    if(x.load(std::memory_order_relaxed))
        ++z;
}

int
main (int argc, char * argv[])
{
    int loop_num = 1;
    if (argc == 2) loop_num = atoi(argv[1]);
    z=0;
    for (int i = 0; i < loop_num; ++i)
    {
        x=false;
        y=false;
        std::thread a(write_x_then_y);
        std::thread b(read_y_then_x);
        a.join();
        b.join();
    }

    std::cout << "z = " << z.load() << std::endl;
    return ZERO;
}
