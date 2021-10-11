#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <atomic>

#define ZERO 0

class interrupt_flag
{
    std::atomic<bool> flag{false};
public:
    void set(){ flag = true; }
    bool is_set() const { return flag.load(); }
};
thread_local interrupt_flag this_thread_interrupt_flag;

class interruptible_thread
{
    std::thread internal_thread;
    interrupt_flag* flag;
public:
    template<typename FunctionType>
    interruptible_thread(FunctionType f)
    {
        std::promise<interrupt_flag*> p;
        internal_thread=std::thread([f,&p]{
            p.set_value(&this_thread_interrupt_flag);
            try
            {
                f();
            }
            catch(int e)
            {
                std::cout << "catch exception: " << e << std::endl;
            }  
        });
        flag=p.get_future().get();
    }
    ~interruptible_thread()
    {
        if (internal_thread.joinable())
            internal_thread.join();
    }
    void interrupt()
    {
        if(flag)
        {
            flag->set();
        }
    }
};

int
thread_interrupted()
{
    std::cout << "thread interrupted!" << std::endl;
    return -1;
}

int
main (int, char * [])
{
    interruptible_thread int_thread ([](){
        while(true){
            if (this_thread_interrupt_flag.is_set())
            {
                std::cout << "flag is set" << std::endl;
                throw thread_interrupted();
            }
        }
    });
    int_thread.interrupt();
}