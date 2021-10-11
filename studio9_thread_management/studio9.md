# STUDIO 9 : Advanced Thread Management Studio

## Q1:

Bingxin Liu

## Q2:

```c++
// CODE:
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
```

```shell
# OUTPUT:
[bingxin.liu@linuxlab009 studio9_thread_management]$ g++ -Wall -std=c++11 -pthread -latomic ./thread_management.cpp
[bingxin.liu@linuxlab009 studio9_thread_management]$ ./a.out
flag is set
thread interrupted!
catch exception: -1
[bingxin.liu@linuxlab009 studio9_thread_management]$
```

We set calling the interrupt function in the main thread, and try to catch the exception in the interruptible thread.

The lambda function repeatedly checks the flag, if the flag is set, we throw a exception, which is caught by the interruptible thread and prints out the content of the exception.

## Q3:

```c++
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

#define ZERO 0

std::mutex future_mutex;

unsigned int
thread_interrupted(unsigned int n)
{
    std::cout << "thread interrupted!" << std::endl;
    return n;
}

class interrupt_flag
{
    std::atomic<bool> flag{false};
public:
    void set(){  flag = true; }
    bool is_set() const { return flag.load(); }
};
thread_local interrupt_flag this_thread_interrupt_flag;

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

class interruptible_thread
{
    std::thread internal_thread;
    interrupt_flag* flag;
public:
    template<typename FunctionType>
    interruptible_thread(FunctionType f)
    {
        std::promise<interrupt_flag*> p;
        internal_thread=std::thread([&f, &p]{
            p.set_value(&this_thread_interrupt_flag);
            try
            {
                srand((unsigned) time(NULL));
                f((unsigned int)(rand() % 10));
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
    void join()
    {
        if (internal_thread.joinable())
            internal_thread.join();
    }
};

int
main (int, char * [])
{

    std::vector<interruptible_thread* > threads;
    std::vector<std::future<unsigned int> > futures;

    for (int i = 0; i < 10; ++i)
    {
        srand((unsigned) time(NULL));
        std::packaged_task<unsigned int(unsigned int)>task(fib);
        futures.push_back(task.get_future());

        interruptible_thread t(std::move(task));
        threads.push_back(&t);

        
    }

    while (!this_thread_interrupt_flag.is_set())
    {
        for (int i = 0; i < 10; ++i)
        {
            std::lock_guard<std::mutex> lk(future_mutex);
            if (futures[i].wait_for(std::chrono::milliseconds(1))
                == std::future_status::ready)
            {
                this_thread_interrupt_flag.set();
                break;
            }
                
        }
    }

    for (int i = 0; i < 10; i++)
    {
        std::cout << i << ":" << futures[i].get();
    }
    std::cout << std::endl;

    return ZERO;

}
```

```shell
# OUTPUT:
[bingxin.liu@linuxlab009 studio9_thread_management]$ ./a.out
0:81:82:83:84:85:86:87:88:89:8
```

## Q4:

