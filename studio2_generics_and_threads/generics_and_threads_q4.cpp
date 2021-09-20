#include <iostream>
#include <iterator>
#include <algorithm>

#define ZERO 0

void print_range(int*);

int
main (int, char * [])
{
  int arr[][3] = {{-2, 19, 80}, {-47, 80, 80}, {-2, 31, 27}};
  int * start = *arr;
  int * end = *arr + sizeof(*arr) / sizeof(int);
  // debug
  std::cout << "sizeof(arr) = " << sizeof(arr) << " sizeof(int) = " << sizeof(int) << " sizeof(int*) = " << sizeof(int*) << std::endl; 
  std::cout << sizeof(arr) / sizeof(int) << std::endl;
  std::cout << sizeof(arr) / sizeof(int*) << std::endl;
  std::ostream_iterator<int> output_iterator(std::cout, "\t");
  std::copy(start, end, output_iterator);
  std::cout << std::endl;
  return ZERO;
}
