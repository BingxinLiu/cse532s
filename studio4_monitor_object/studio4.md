# STUDIO 4 : Monitor Object

## Q1:

Bingxin Liu

## Q2:

```c++
// CODE: monitor_object.hpp
#include <iostream>
#include <list>
#include <memory>

template<class T>
class synchronized_list
{
    std::list<T> list_;
public:
    synchronized_list()
    {
        list_ = std::list<T>();
    }
    void push_back(const T new_data)
    {
        list_.push_back(new_data);
    }
    std::shared_ptr<T> pop_back()
    {
        if (list_.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> result = std::make_shared<T>(list_.back());
        list_.pop_back();
        return result;
    }
    void push_front(const T new_data)
    {
        list_.push_front(new_data);
    }
    std::shared_ptr<T> pop_front()
    {
        if (list_.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> result = std::make_shared<T>(list_.front());
        list_.pop_front();
        return result;
    }
};
```

```c++
// CODE: monitor_object.cpp
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
```

```shell
[bingxin.liu@linuxlab006 studio4_monitor_object]$ g++ -Wall -std=c++11 -pthread ./monitor_object.cpp
[bingxin.liu@linuxlab006 studio4_monitor_object]$ ./a.out
pop front:
9	8	7	6	5	4	3	2	1	0
[bingxin.liu@linuxlab006 studio4_monitor_object]$
```

## Q3:

```c++
// CODE:
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
```

```shell
# OUTPUT:
[bingxin.liu@linuxlab006 studio4_monitor_object]$ ./a.out
pop back:
0	1	2	3	4	7	6	5	8	9
[bingxin.liu@linuxlab006 studio4_monitor_object]$ ./a.out
pop back:
0	1	2	3	4	6	7	5	8	9
[bingxin.liu@linuxlab006 studio4_monitor_object]$
```

We don't know the exact order of running each thread, therefore, the result varies according to different situation.

## Q4:

```c++
// CODE:
    std::cout << "pop back:" << std::endl;
    while (!sync_list.empty())
        threads.push_back(std::thread([&](){
            std::cout << *(sync_list.pop_back().get()) << "\t";
        }));
    std::for_each(threads.begin(), threads.end(),\
        std::mem_fn(&std::thread::join));
    std::cout << std::endl;
```

```shell
# OUTPUT:
[bingxin.liu@linuxlab006 studio4_monitor_object]$ g++ -Wall -std=c++11 -pthread ./monitor_object_q4.cpp
[bingxin.liu@linuxlab006 studio4_monitor_object]$ ./a.out
pop back:
Segmentation fault (core dumped)
[bingxin.liu@linuxlab006 studio4_monitor_object]$
```

Apparently, we will have segmentation fault. This is caused by two or more threads trying to pop the same item, which is a race condition.

## Q5:

```c++
// CODE:
#include <iostream>
#include <list>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

template<class T>
class synchronized_list
{
    std::list<T> list_;
    std::mutex mutex_;
    std::condition_variable cond_var;
public:
    synchronized_list()
    {
        list_ = std::list<T>();
    }
    void push_back(const T new_data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        list_.push_back(new_data);
        cond_var.notify_one();
    }
    std::shared_ptr<T> pop_back()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var.wait(lock, [this]{return !list_.empty();});
        std::shared_ptr<T> result = std::make_shared<T>(list_.back());
        list_.pop_back();
        return result;
    }
    void push_front(const T new_data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        list_.push_front(new_data);
        cond_var.notify_one();
    }
    std::shared_ptr<T> pop_front()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var.wait(lock, [this]{return !list_.empty();});
        std::shared_ptr<T> result = std::make_shared<T>(list_.front());
        list_.pop_front();
        return result;
    }
    bool empty() {
        std::lock_guard<std::mutex> lock(mutex_);
        return this->list_.empty();
    }
};
```

```shell
# OUTPUT:
[bingxin.liu@linuxlab006 studio4_monitor_object]$ g++ -Wall -std=c++11 -pthread ./monitor_object_q5.cpp
[bingxin.liu@linuxlab006 studio4_monitor_object]$ ./a.out
pop back:
0	1	2	3	4	5	8	7	6	9
[bingxin.liu@linuxlab006 studio4_monitor_object]$
```

Now, we can find that there isn't any `Segmentation fault` reported. This is because **`std::mutex`**, a **`std::unique_lock`**, and a **`std::condition_variable`** help us figure out the order of push and pop and avoid race condition.

## Q6:

```shell
# OUTPUT:
[bingxin.liu@linuxlab006 studio4_monitor_object]$ g++ -Wall -std=c++11 -pthread ./monitor_object_q6.cpp
[bingxin.liu@linuxlab006 studio4_monitor_object]$ ./a.out
push front: 5
push back: 0
push front: 8
push front: 9
pop back: 0
push back: 3
pop back: 3
push front: 6
pop back: 5
pop back: 8
push back: 4
pop back: 4
push back: 1
push back: 2
pop back: 2
push front: 7
pop back: 1
pop back: 9
```

```c++
// CODE:
void push_back(const T new_data)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (high_ > 0)
        {
            cond_var_high_.wait(lock, [this]{ return list_.size() < high_; });
        }
        list_.push_back(new_data);
        cond_var_low_.notify_one();
        std::cout << "push back: " << new_data << std::endl;
        lock.unlock();
    }
std::shared_ptr<T> pop_back()
{
  std::unique_lock<std::mutex> lock(mutex_);
  cond_var_low_.wait(lock, [this]{ return list_.size() > low_; });
  std::shared_ptr<T> result = std::make_shared<T>(list_.back());
  list_.pop_back();
  cond_var_high_.notify_one();
  std::cout << "pop back: " << *(result.get()) << std::endl;
  return result;
}

synchronized_list<int> sync_list(4, 2);
```

We only shows `push_back` and `pop_back` since `push_front` and `pop_front` are the same. For result we can observe that there are ten push operations and eight operations, because we set the minimum number of items in the queue should be two. Also, we can notice that when we push four items at the beginning, only after popping at least one items can we push more item.