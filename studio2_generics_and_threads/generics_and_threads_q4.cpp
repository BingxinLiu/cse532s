#include <iostream>
#include <iterator>
#include <algorithm>
#include<numeric>

#define ZERO 0

int
main (int, char * [])
{
    int arr[][4] = {{-2, 19, 80, 21}, {-47, 80, 80, 32}, {-2, 31, 27, 57}};
    int range_size = sizeof(arr) / sizeof(*arr);
    for (int i = 0; i < range_size; ++i)
    {
        int * start = *(arr + i);
        int * end = start + sizeof(*arr) / sizeof(int);
        std::ostream_iterator<int> output_iterator(std::cout, "\t");
        std::copy(start, end, output_iterator);
        int sum = std::accumulate(start, end, 0);
        std::cout << "sum = " << sum << std::endl;
    }
    return ZERO;
}
