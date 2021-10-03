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
    std::condition_variable cond_var_high_;
    std::condition_variable cond_var_low_;
    std::size_t high_;
    std::size_t low_;
public:
    synchronized_list()
    {
        list_ = std::list<T>();
        high_ = 0;
        low_ = 0;
    }
    synchronized_list(std::size_t high, std::size_t low)\
        : high_(high), low_(low)
    {
        list_ = std::list<T>();
    }
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
    void push_front(const T new_data)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (high_ > 0)
            cond_var_high_.wait(lock, [this]{ return list_.size() < high_; });
        list_.push_front(new_data);
        cond_var_low_.notify_one();
        std::cout << "push front: " << new_data << std::endl;
        lock.unlock();
    }
    std::shared_ptr<T> pop_front()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_low_.wait(lock, [this]{ return list_.size() > low_; });
        std::shared_ptr<T> result = std::make_shared<T>(list_.front());
        list_.pop_front();
        cond_var_high_.notify_one();
        std::cout << "pop front: " << *(result.get()) << std::endl;
        return result;
    }
    bool empty() {
        std::lock_guard<std::mutex> lock(mutex_);
        return this->list_.empty();
    }
};