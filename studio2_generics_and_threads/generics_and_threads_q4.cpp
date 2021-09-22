#include <iostream>
#include <iterator>
#include <algorithm>

#define ZERO 0

void print_range(int*);

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
        std::cout << std::endl;
    }
    return ZERO;
}
