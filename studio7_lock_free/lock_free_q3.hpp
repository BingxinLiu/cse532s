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
