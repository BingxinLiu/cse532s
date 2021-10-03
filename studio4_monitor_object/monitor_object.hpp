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
    bool empty() { return this->list_.empty(); }
};