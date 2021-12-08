#include "threadsafe_io.hpp"

threadsafe_io* operator<<(threadsafe_io* io, const std::string str)
{
    io->print(str);
    return io;
}

void
threadsafe_io::print(const std::string str)
{
    std::lock_guard<std::mutex> lk(this->io_mutex_);
    this->os_ << str;
}

const std::string
threadsafe_io::get()
{
    std::string result;
    std::lock_guard<std::mutex> lk(this->io_mutex_);
    getline(this->is_, result);
    return result;
}

threadsafe_io*
threadsafe_io::get_instance()
{
    if (io_ == nullptr) io_ = new threadsafe_io();
    return io_;
}



