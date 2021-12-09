#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <list>

#ifndef THREADSAFE_IO_HPP
#define THREADSAFE_IO_HPP

class threadsafe_io
{
    std::ostream& os_ = std::cout;
    std::istream& is_ = std::cin;
    std::mutex io_mutex_;
    static threadsafe_io* io_;
    threadsafe_io(){}
public:
    threadsafe_io(threadsafe_io &other) = delete;
    void operator=(const threadsafe_io&) = delete;
    
    static threadsafe_io *get_instance()
    {
        if (threadsafe_io::io_ == nullptr) threadsafe_io::io_ = new threadsafe_io();
        return threadsafe_io::io_;
    }

    template<class T>
    threadsafe_io& operator<<(const T t)
    {
        std::lock_guard<std::mutex> lk(this->io_mutex_);
        this->os_ << t;
        return *this;
    }

    void
    flush()
    {
        std::lock_guard<std::mutex> lk(this->io_mutex_);
        this->os_ << std::endl;
    }
};

#endif