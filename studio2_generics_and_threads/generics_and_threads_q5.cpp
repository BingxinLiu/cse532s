#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <thread>
#include <vector>

#define ZERO 0

void print_range(int* result_araray, int* start, int* end, int index)
{
    std::ostream_iterator<int> output_iterator(std::cout, "\t");
    std::copy(start, end, output_iterator);
    int sum = std::accumulate(start, end, 0);
    std::cout << "sum = " << sum << std::endl;
    result_araray[index] = sum;
}

int
main (int, char * [])
{
    int arr[][4] = {{-2, 19, 80, 21}, {-47, 80, 80, 32}, {-2, 31, 27, 57}};
    int result_array[3];
    int range_size = sizeof(arr) / sizeof(*arr);
    std::vector<std::thread> threads;
    for (int i = 0; i < range_size; ++i)
    {
        int * start = *(arr + i);
        int * end = start + sizeof(*arr) / sizeof(int);
        std::thread t(print_range, result_array, start, end, i);
        t.join();
    }
    int * start = result_array;
    int * end = result_array + sizeof(result_array) / sizeof(int);
    std::cout << "The result of accumulating each range:\n";
    std::ostream_iterator<int> output_iterator(std::cout, "\t");
    std::copy(start, end, output_iterator);
    std::cout << std::endl;


    return ZERO;
}
