#pragma once
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

class interrupt_flag
{
    std::atomic<bool> flag;
    std::condition_variable* thread_cond;
    std::mutex set_clear_mutex;

public:
    interrupt_flag():
        thread_cond(0)
    {}

    void set()
    {
        flag.store(true,std::memory_order_relaxed);
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        if(thread_cond)
        {
            thread_cond->notify_all();
        }
    }  
    bool is_set() const
    {
        return flag.load(std::memory_order_relaxed);
    }

    void set_condition_variable(std::condition_variable& cv)
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond=&cv;
    }

    void clear_condition_variable()
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond=0;
    }

    struct clear_cv_on_destruct
    {
        ~clear_cv_on_destruct()
        {
            this_thread_interrupt_flag.clear_condition_variable();
        }
    };
};

void interruptible_wait(std::condition_variable& cv,
                        std::unique_lock<std::mutex>& lk)
{
    interruption_point2();
    interrupt_flag this_thread_interrupt_flag;
    //    this_thread_interrupt_flag.set_condition_variable(cv);
    interrupt_flag::clear_cv_on_destruct guard;
    interruption_point2();
    cv.wait_for(lk,std::chrono::milliseconds(1));
    interruption_point2();
}

void interruption_point2(){
    if (this_thread_interrupt_flag.is_set())
    {
        throw 1;
    }
};

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