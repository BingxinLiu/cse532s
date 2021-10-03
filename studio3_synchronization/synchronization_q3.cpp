#include <iostream>
#include <vector>
#include <numeric>
#include <thread>

#define ZERO 0

class vector_wrapper
{
    std::vector<int> vector_;
public:
    vector_wrapper& operator<<(const int input)
    {
        vector_.push_back(input);
        return *this;
    }
    int operator() ()
    {
        if (vector_.size() == 0) return 0;
        return std::accumulate(vector_.begin(), vector_.end(), 0);
    }
};

int
main(int, char * [])
{
    vector_wrapper vw;
    std::thread t1([&](){ vw << 1 << 2 << 3 << 4; });
    std::thread t2([&](){ std::cout << vw() << std::endl; });
    std::thread t3([&](){ vw << 5 << 6 << 7 << 8; });
    std::thread t4([&](){ std::cout << vw() << std::endl; });
    std::thread t5([&](){ vw << 9 << 10 << 11 << 12; });
    std::thread t6([&](){ std::cout << vw() << std::endl; });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return ZERO;
}