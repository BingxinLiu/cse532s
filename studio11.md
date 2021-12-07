# STUDIO 11 : Half-Sync/Half-Async and Leader/Followers Studio

## Q1:

Bingxin Liu

## Q2:

```c++
// CODE:
#include <iostream>

#define SUCCESS 0

using namespace std;

bool
is_prime(unsigned int n)
{
    if ( n == 0 || n == 1 ) return false;
    for ( unsigned int i = 2; i <= n / 2; ++i )
    {
        if ( n % i == 0 ) return false;
    }
    return true;
}


int 
main(int argc, char* argv[])
{
    cout << " 5 " << (is_prime(5) ? "is a prime number" : "is not a prime number") << endl; 
    cout << " 51 " << (is_prime(51) ? "is a prime number" : "is not a prime number") << endl;
    cout << " 23 " << (is_prime(23) ? "is a prime number" : "is not a prime number") << endl;
    cout << " 86 " << (is_prime(86) ? "is a prime number" : "is not a prime number") << endl;

    return SUCCESS;
}
```

```shell
# OUTPUT:
[bingxin.liu@linuxlab005 studio11_hsha_lf]$ ./hsha_lf
 5 is a prime number
 51 is not a prime number
 23 is a prime number
 86 is not a prime number
[bingxin.liu@linuxlab005 studio11_hsha_lf]$
```

## Q3:

```c++
// CODE:
// sync_os.hpp
#include <iostream>
#include <mutex>

using namespace std;

class Sync_Output_Stream
{
    ostream& os;
    mutex print_mutex;

public:
    Sync_Output_Stream(ostream& os = cout);
    void print(const string& str);
};

// sync_os.cpp
#include "sync_os.hpp"

Sync_Output_Stream::Sync_Output_Stream(ostream& os) \
    : os(os)
{}

void 
Sync_Output_Stream::print(const string& str)
{
    lock_guard<mutex> print_lock(print_mutex);
    this->os << str << endl;
}

// threadsafe_queue.hpp
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class threadsafe_queue
{
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cv;
public:
    threadsafe_queue() {}

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cv.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cv.wait(lk, [this]{ return !data_queue.empty(); });
        value = std::move( data_queue.front() );
        data_queue.pop();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if ( data_queue.empty() ) return false;
        value = std::move( data_queue.front() );
        data_queue.pop();
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if ( data_queue.empty() ) return std::shared_ptr<T>();
        std::shared_ptr<T> res( std::make_shared<T>(std::move(data_queue.front())) );
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

};

// thread_pool.hpp
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
    threadsafe_queue<std::pair<std::function<bool(unsigned int)>, unsigned int> > work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;
    Sync_Output_Stream sync_os;

    void worker_thread()
    {
        while(!done)
        {
            std::shared_ptr<std::pair<std::function<bool(unsigned int)>, unsigned int> > task(std::move(work_queue.try_pop()));
            if(task != nullptr)
            {
                stringstream ss;
                if ( (*task).first( (*task).second ) != true )
                    ss << " " << (*task).second << " is not a prime number\n";
                else
                    ss << " " << (*task).second << " is a prime number\n";
                sync_os.print(ss.str());
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
public:
    thread_pool():
        done(false),joiner(threads)
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

    void submit(std::pair<std::function<bool(unsigned int)>, unsigned int> func_and_arg)
    {
        work_queue.push(func_and_arg);
    }
};

// hsha_lf_q3.cpp
#include <iostream>
#include "thread_pool.hpp"

#define SUCCESS 0

using namespace std;

bool
is_prime(unsigned int n)
{
    if ( n == 0 || n == 1 ) return false;
    for ( unsigned int i = 2; i <= n / 2; ++i )
    {
        if ( n % i == 0 ) return false;
    }
    return true;
}


int 
main(int argc, char* argv[])
{
    thread_pool thread_pool;

    thread_pool.submit(move(make_pair(&is_prime, 5)));
    thread_pool.submit(move(make_pair(&is_prime, 12)));
    thread_pool.submit(move(make_pair(&is_prime, 97)));
    thread_pool.submit(move(make_pair(&is_prime, 32)));
    thread_pool.submit(move(make_pair(&is_prime, 37)));
    thread_pool.submit(move(make_pair(&is_prime, 61)));
    thread_pool.submit(move(make_pair(&is_prime, 7)));

    return SUCCESS;
}
```

```shell
#OUTPUT:

[bingxin.liu@linuxlab002 studio11_hsha_lf]$ ./hsha_lf_q3
 5 is a prime number

 12 is not a prime number

 97 is a prime number

 32 is not a prime number

 37 is a prime number

 61 is a prime number

 7 is a prime number

[bingxin.liu@linuxlab002 studio11_hsha_lf]$
```

## Q4:

```shell
# OUTPUT:
[bingxin.liu@linuxlab004 studio11_hsha_lf]$ ./hsha_lf_q4
[47452898617088] becomes the leader

[47452898617088] release the leader to calculate 5

 5 is a prime number

[47452915427072] becomes the leader

 12 is not a prime number

[47452915427072] becomes the leader

[47452915427072] release the leader to calculate 1000861

[47452894414592] becomes the leader

[47452894414592] release the leader to calculate 97

 97 is a prime number

[47452892313344] becomes the leader

 32 is not a prime number

[47452892313344] becomes the leader

[47452892313344] release the leader to calculate 999999893

[47452928034560] becomes the leader

[47452928034560] release the leader to calculate 37

 37 is a prime number

[47452886009600] becomes the leader

[47452886009600] release the leader to calculate 61

 61 is a prime number

[47452917528320] becomes the leader

[47452917528320] release the leader to calculate 17

 17 is a prime number

 1000861 is a prime number

 999999893 is a prime number

[bingxin.liu@linuxlab004 studio11_hsha_lf]$
```

```c++
// CODE:
// thread_pool.hpp
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
```

## Q5:

The speed of responsive of the program was very quick when I typed in the numbers manually.
The speed of responsive of the program was a little bit slower than previous exercise.
I think this situation is caused by the property of LF pattern, since in LF pattern, there is less competition for the thread queue but the leader has more job to do.

```shell
# OUTPUT FOR INTERACTIVE:
32
[47246804203264] becomes the leader

 32 is not a prime number

43
[47246768482048] becomes the leader

[47246768482048] release the leader to calculate 43

 43 is a prime number


65
[47246804203264] becomes the leader

[47246804203264] release the leader to calculate 65

 65 is not a prime number

 999999893 is a prime number

Q
[bingxin.liu@linuxlab004 studio11_hsha_lf]$


# OUTPUT FOR FILE
[bingxin.liu@linuxlab009 studio11_hsha_lf]$ ./hsha_lf_q5 -f prime_numbers
[47036673177344] becomes the leader

 12 is not a prime number

[47036673177344] becomes the leader

[47036673177344] release the leader to calculate 1000861

[47036675278592] becomes the leader

[47036675278592] release the leader to calculate 97

 97 is a prime number

[47036652164864] becomes the leader

 32 is not a prime number

[47036652164864] becomes the leader

[47036652164864] release the leader to calculate 999999893

[47036650063616] becomes the leader

[47036650063616] release the leader to calculate 37

 37 is a prime number

[47036656367360] becomes the leader

[47036656367360] release the leader to calculate 61

 61 is a prime number

[47036671076096] becomes the leader

[47036671076096] release the leader to calculate 17

 17 is a prime number

 1000861 is a prime number

 999999893 is a prime number

[bingxin.liu@linuxlab009 studio11_hsha_lf]$
```

```c++
// CODE FOR MAIN:
#include <iostream>
#include <string>
#include <fstream>
#include "thread_pool.hpp"

#define SUCCESS 0

using namespace std;

bool
is_prime(long long n)
{
    if ( n == 0 || n == 1 ) return false;
    for ( long long i = 2; i <= n / 2; ++i )
    {
        if ( n % i == 0 ) return false;
    }
    return true;
}


int 
main(int argc, char* argv[])
{
    bool interactive = false;

    if (argc < 2 || argc > 3)
    {
        std::cout << "USAGE: ./hsha_If_q5 {-i|-f} [-f <filename>]" << endl;
        return EINVAL;
    }
    
    string config_arg(argv[1]);
    string input_file;
    string input_str;
    long long input_number;

    if (config_arg == string("-i")) interactive = true;
    else if (config_arg != string("-f") || argc != 3)
    {
        std::cout << "USAGE: ./hsha_If_q5 {-i|-f} [-f <filename>]" << endl;
        return EINVAL;
    } else
    {
        input_file = string(argv[2]);
    }

    thread_pool thread_pool;

    if (interactive)
    {
        std::cout << "Please input numbers" << endl;
        for(;;)
        {
            std::cin >> input_str;
            try
            {
                input_number = stoll(input_str);
            } catch(...)
            {
                if (input_str == "Q") break;
                std::cout << "invalid input" << input_str << endl;
                continue;
            }
            thread_pool.submit(move(make_pair(&is_prime, input_number)));

        }
    } else
    {
        string line;
        ifstream myfile (input_file);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                try
                {
                    input_number = stoll(line);
                } catch(...)
                {
                    if (line == "Q") break;
                    std::cout << "invalid input" << line << endl;
                    continue;
                }
                thread_pool.submit(move(make_pair(&is_prime, input_number)));
                
            }
            myfile.close();
        }
        else std::cout << "Unable to open file"; 

    }


    return SUCCESS;
}
```