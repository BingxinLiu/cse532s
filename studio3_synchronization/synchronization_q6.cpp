#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <mutex>

#define ZERO 0

std::mutex print_mutex_;

template<class mutex_type>
class vector_wrapper
{
    mutex_type mutex_;
    std::vector<int> vector_;
public:
    vector_wrapper& operator<<(const int input)
    {
        std::lock_guard<mutex_type> lock(mutex_);
        vector_.push_back(input);
        return *this;
    }
    int operator() ()
    {
        std::lock_guard<mutex_type> lock(mutex_);
        if (vector_.size() == 0) return 0;
        return std::accumulate(vector_.begin(), vector_.end(), 0);
    }
    int factorial_of_sum()
    {
        std::lock_guard<mutex_type> lock(mutex_);
        int result = (*this)();
        int sum = 1;
        for (int i = 1; i <= result; ++i)
            sum *= i;
        return sum;
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
    vector_wrapper<std::recursive_mutex> vw;
    std::thread t1([&](){ vw << 1 << 2 << 3 << 4; });
    std::thread t3([&](){ vw << 5 << 6 << 7 << 8; });

    std::thread nested_lock([&](){ 
        std::cout   << "The factorial_of_sum is "\
                    << vw.factorial_of_sum()\
                    << ", right now." << std::endl; });
    std::thread t5([&](){ vw << 9 << 10 << 11 << 12; });
    t1.join();
    t3.join();
    nested_lock.join();
    t5.join();

    return ZERO;
}