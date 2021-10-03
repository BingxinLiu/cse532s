#include <iostream>
#include <vector>
#include <numeric>

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
    std::cout << (vw << 1 << 2 << 3 << 4) () << std::endl;
    std::cout << (vw << 5 << 6 << 7 << 8) () << std::endl;
    std::cout << (vw << 9 << 10 << 11 << 12) () << std::endl;

    return ZERO;
}