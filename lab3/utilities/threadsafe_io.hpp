#pragma once

#include <iostream>
#include <mutex>
#include <string>

class threadsafe_io
{
    std::ostream& os_ = std::cout;
    std::istream& is_ = std::cin;
    std::mutex io_mutex_;
    static threadsafe_io* io_;
    threadsafe_io();
public:
    threadsafe_io(threadsafe_io &) = delete;
    void operator=(const threadsafe_io&) = delete;
    
    static threadsafe_io *get_instance();

    void print(const std::string str);
    friend threadsafe_io* operator<<(threadsafe_io*, const std::string);

    const std::string get(void);
};

threadsafe_io* threadsafe_io::io_ = nullptr;