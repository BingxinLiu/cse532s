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