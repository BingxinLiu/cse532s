# STUDIO 3 : Synchronization

## Q1:

Bingxin Liu

## Q2:

```c++
// code:
#include <iostream>
#include <vector>
#include <numeric>

#define ZERO 0

class vector_wrapper
{
    std::vector<int> vector_;
public:
    vector_wrapper& operator<<(const int input)
    {
        vector_.push_back(input);
        return *this;
    }
    int operator() ()
    {
        if (vector_.size() == 0) return 0;
        return std::accumulate(vector_.begin(), vector_.end(), 0);
    }
};

int
main(int, char * [])
{
    vector_wrapper vw;
    std::cout << (vw << 1 << 2 << 3 << 4) () << std::endl;
    std::cout << (vw << 5 << 6 << 7 << 8) () << std::endl;
    std::cout << (vw << 9 << 10 << 11 << 12) () << std::endl;

    return ZERO;
}
```

```shell
#ouput:
[bingxin.liu@linuxlab009 studio3_synchronization]$ g++ -Wall -std=c++11 -pthread ./synchronization.cpp
[bingxin.liu@linuxlab009 studio3_synchronization]$ ./a.out
10
36
78
[bingxin.liu@linuxlab009 studio3_synchronization]$
```

## Q3:

```c++
// CODE:
#include <iostream>
#include <vector>
#include <numeric>
#include <thread>

#define ZERO 0

class vector_wrapper
{
    std::vector<int> vector_;
public:
    vector_wrapper& operator<<(const int input)
    {
        vector_.push_back(input);
        return *this;
    }
    int operator() ()
    {
        if (vector_.size() == 0) return 0;
        return std::accumulate(vector_.begin(), vector_.end(), 0);
    }
};

int
main(int, char * [])
{
    vector_wrapper vw;
    std::thread t1([&](){ vw << 1 << 2 << 3 << 4; });
    std::thread t2([&](){ std::cout << vw() << std::endl; });
    std::thread t3([&](){ vw << 5 << 6 << 7 << 8; });
    std::thread t4([&](){ std::cout << vw() << std::endl; });
    std::thread t5([&](){ vw << 9 << 10 << 11 << 12; });
    std::thread t6([&](){ std::cout << vw() << std::endl; });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return ZERO;
}
```

```shell
# OUTPUT
[bingxin.liu@linuxlab005 studio3_synchronization]$ g++ -Wall -std=c++11 -pthread ./synchronization_q3.cpp
[bingxin.liu@linuxlab005 studio3_synchronization]$ ./a.out
Segmentation fault (core dumped)
[bingxin.liu@linuxlab005 studio3_synchronization]$
```

The program throws a segmentation fault exception, this may be caused in a situation that one thread is using iterator to traverse the vector, and another thread is keeping inserting elements into the same vector, which is a race condition and causes a undefined behaviour in c++.



## Q4:

```c++
// CODE:
std::mutex print_mutex_;

class vector_wrapper
{
    std::mutex mutex_;
    std::vector<int> vector_;
public:
    vector_wrapper& operator<<(const int input)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.push_back(input);
        return *this;
    }
    int operator() ()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (vector_.size() == 0) return 0;
        return std::accumulate(vector_.begin(), vector_.end(), 0);
    }
};

void print(std::ostream& os, int result)
{
    std::lock_guard<std::mutex> lock(print_mutex_);
    os << result << std::endl;
}
```

```shell
[bingxin.liu@linuxlab005 studio3_synchronization]$ ./a.out
36
36
78
[bingxin.liu@linuxlab005 studio3_synchronization]$ ./a.out
45
45
78
[bingxin.liu@linuxlab005 studio3_synchronization]$ ./a.out
3
52
52
[bingxin.liu@linuxlab005 studio3_synchronization]$
```

In this case, the program could run without throws a segmentation fault exception.However, we still aren't able to get a determinate state of the result of running the program. This is because in our multi-threads program, we don't know the relative order of several insertions and accumulations operations. As a result the output may seems weird.

## Q5:

```shell
// Output
[bingxin.liu@linuxlab006 studio3_synchronization]$ g++ -Wall -std=c++11 -pthread ./synchronization_q5.cpp
[bingxin.liu@linuxlab006 studio3_synchronization]$ ./a.out
The factorial_of_sum is 

[bingxin.liu@linuxlab006 studio3_synchronization]$ ./a.out
The factorial_of_sum is 

```

As we can see, the expected output should including something like "The factorial_of_sum is XXX, right now", but the program is trapped when it is calling the `factorial_of_sum` function. The thread and its thread function are shown below:

```c++
std::thread nested_lock([&](){ 
        std::cout   << "The factorial_of_sum is "\
                    << vw.factorial_of_sum()\
                    << ", right now." << std::endl; });
```

This is because when we called factorial_of_sum(), we got a mutex at first, but we then called `function operation` to get the sum of the vector right now, which was tring to obtain the mutex we already obtained. Consequently, we cause the dead lock.

## Q6:

```shell
// Output
[bingxin.liu@linuxlab006 studio3_synchronization]$ ./a.out
The factorial_of_sum is 479001600, right now.
```

```c++
#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <mutex>

#define ZERO 0

std::mutex print_mutex_;

template<class mutex_type>
class vector_wrapper
{
    mutex_type mutex_;
    std::vector<int> vector_;
public:
    vector_wrapper& operator<<(const int input)
    {
        std::lock_guard<mutex_type> lock(mutex_);
        vector_.push_back(input);
        return *this;
    }
    int operator() ()
    {
        std::lock_guard<mutex_type> lock(mutex_);
        if (vector_.size() == 0) return 0;
        return std::accumulate(vector_.begin(), vector_.end(), 0);
    }
    int factorial_of_sum()
    {
        std::lock_guard<mutex_type> lock(mutex_);
        int result = (*this)();
        int sum = 1;
        for (int i = 1; i <= result; ++i)
            sum *= i;
        return sum;
    }
};

void print(std::ostream& os, int result)
{
    std::lock_guard<std::mutex> lock(print_mutex_);
    os << result << std::endl;
}



int
main(int, char * [])
{
    vector_wrapper<std::recursive_mutex> vw;
    std::thread t1([&](){ vw << 1 << 2 << 3 << 4; });
    std::thread t3([&](){ vw << 5 << 6 << 7 << 8; });

    std::thread nested_lock([&](){ 
        std::cout   << "The factorial_of_sum is "\
                    << vw.factorial_of_sum()\
                    << ", right now." << std::endl; });
    std::thread t5([&](){ vw << 9 << 10 << 11 << 12; });
    t1.join();
    t3.join();
    nested_lock.join();
    t5.join();

    return ZERO;
}
```

After we instantiate the class object with **`std::recursive_mutex`**, we avoid the dead lock. The output normally printout the result. This is becausestd::recursive_mutex allow us to recursively require a lock and will not cause dead-lock.
