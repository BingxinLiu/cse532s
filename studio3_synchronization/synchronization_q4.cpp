#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <mutex>

#define ZERO 0

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

int
main(int, char * [])
{
    vector_wrapper vw;
    std::thread t1([&](){ vw << 1 << 2 << 3 << 4; });
    std::thread t2([&](){ 
        int result = vw();
        print(std::cout, result);
    });
    std::thread t3([&](){ vw << 5 << 6 << 7 << 8; });
    std::thread t4([&](){ 
        int result = vw();
        print(std::cout, result); });
    std::thread t5([&](){ vw << 9 << 10 << 11 << 12; });
    std::thread t6([&](){ 
        int result = vw();
        print(std::cout, result); });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return ZERO;
}