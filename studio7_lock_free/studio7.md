# STUDIO 7 : Lock Free Data Structures Studio

## Q1:

Bingxin Liu

## Q2:

The reason why the push method's while loop does not need to repeat the statement just before it that loaded the value of the atomic head pointer into the new node's next pointer is that the `compare_exchange_weak` help us do it automatically for us.

`while(!head.compare_exchange_weak(new_node->next,new_node));`

It first check if head is bit-wise equal to new_node->next. If it is, the method return true and set head pointing to new_node. Otherwise, it set new_node->next with which the head is pointing right now. Therefore, we are not bothered to update manually.

## Q3:

```c++
// CODE: lock_free.hpp
#include <atomic>
#include <memory>
#include <mutex>
#include <iostream>

template<typename T>
class lock_free_stack
{
private:
    // used for std::cout, which is not thread safe
    std::mutex mutex_;
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node(T const& data_):
            data(std::make_shared<T>(data_))
        {}
    };
    std::atomic<node*> head;
public:
    void push(T const& data)
    {
        node* const new_node=new node(data);
        new_node->next=head.load();
        while(!head.compare_exchange_weak(new_node->next,new_node));
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Push: " << data << std::endl;
    }
    std::shared_ptr<T> pop()
    {
        node* old_head=head.load();
        while(old_head &&
              !head.compare_exchange_weak(old_head,old_head->next));
        std::lock_guard<std::mutex> lock(mutex_);
        if (old_head)
            std::cout << "Pop: " << *(old_head->data.get()) << std::endl;
        else
            std::cout << "Pop: " << "null" << std::endl;
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
};
```

```c++
// CODE: lock_free.cpp
#include "lock_free_q3.hpp"

#define ZERO 0

int
main (int, char * [])
{
    lock_free_stack<int> stack;
    stack.push(1);
    stack.pop();
    stack.pop();
    stack.push(3);
    stack.push(7);
    stack.push(2);
    stack.pop();
    stack.pop();
    return ZERO;
}

```

```shell
# OUTPUT:
[bingxin.liu@linuxlab009 studio7_lock_free]$ ./a.out
Push: 1
Pop: 1
Segmentation fault (core dumped)
[bingxin.liu@linuxlab009 studio7_lock_free]$
```

As we can see, Segmentation fault is reported. This may be caused by we newed a node when we push, but didn't delete it.

## Q4:

```shell
# OUTPUT:
[bingxin.liu@linuxlab009 studio7_lock_free]$ ./a.out
thread1_container:
8	6	3	2	0
thread2_container:
9	7	5	1	4
[bingxin.liu@linuxlab009 studio7_lock_free]$
```

According to the observation of the output, we can find that neither of the containers end up with both even and odd numbers, which tells us that we can avoid race condition by leveraging lock free technique, but we can only determine the order of each operation after the program has finished its execution.



## Q5:

```shell
# OUTPUT:
[bingxin.liu@linuxlab009 studio7_lock_free]$ ./a.out
thread1_container:
8	6	5	1	4
thread2_container:
9	7	3	2	0
[bingxin.liu@linuxlab009 studio7_lock_free]$
```

As we can see, the out is the same as the previous one. But if we running the program with the way of Q3, we will find that the program will no longer report `Segmentation fault (core dumped)`. In this case, we use GC to avoid daggling node and leaked memory.