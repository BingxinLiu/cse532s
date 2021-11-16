#include <thread>
#include <vector>
#include <atomic>
#include <string>
#include <sstream>
#include <functional>
#include "sync_os.hpp"
#include "threadsafe_queue.hpp"

struct join_threads
{
    std::vector<std::thread>& threads;
    join_threads(std::vector<std::thread>& threads) \
        : threads(threads)
    {}
    ~join_threads()
    {
        for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it)
        {
            if ( (*it).joinable() )
                (*it).join();
        }
    }
};

class thread_pool
{
    std::atomic_bool done;
    std::atomic_flag has_leader;
    threadsafe_queue<std::pair<std::function<bool(long long)>, long long> > work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;
    Sync_Output_Stream sync_os;

    void worker_thread()
    {
        while(!done)
        {
            while (std::atomic_flag_test_and_set_explicit(&has_leader, std::memory_order_acquire )) {}
            while (true)
            {
                std::shared_ptr<std::pair<std::function<bool(long long)>, long long> > task(std::move(work_queue.try_pop()));
                if(task != nullptr)
                {
                    stringstream ss;
                    ss << "[" << this_thread::get_id() << "] becomes the leader" << endl;
                    sync_os.print(ss.str());
                    ss.clear();
                    long long n = (*task).second;
                    if ( n == 0 || n == 1 || (n % 2) == 0 ) 
                    {
                        stringstream ss;
                        ss << " " << n << " is not a prime number\n";
                        sync_os.print(ss.str());
                        ss.clear();
                        continue;
                    } else 
                    {
                        stringstream ss;
                        ss << "[" << this_thread::get_id() << "] release the leader to calculate " << n << endl;
                        sync_os.print(ss.str());
                        std::atomic_flag_clear_explicit(&has_leader, std::memory_order_release);
                        stringstream sss;
                        if ( (*task).first( n ) != true )
                            sss << " " << n << " is not a prime number\n";
                        else
                            sss << " " << (*task).second << " is a prime number\n";
                        sync_os.print(sss.str());
                        sss.clear();
                        break;

                    }
                }
                std::atomic_flag_clear_explicit(&has_leader, std::memory_order_release);
                break;
            }
            std::this_thread::yield();
        }
    }
public:
    thread_pool():
        done(false),has_leader(false),joiner(threads)
    {
        unsigned const thread_count=std::thread::hardware_concurrency();
        try
        {
            for(unsigned i=0;i<thread_count;++i)
            {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread,this));
            }
        }
        catch(...)
        {
            done=true;
            throw;
        }
    }

    ~thread_pool()
    {
        done=true;
    }

    void submit(std::pair<std::function<bool(long long)>, long long> func_and_arg)
    {
        work_queue.push(func_and_arg);
    }
};