#include <iostream>
#include <iterator>
#include <algorithm>

#define ZERO 0

int
main (int, char * [])
{
  int arr[] = {-2, 19, 80, -47, 80, 80, -2};
  int * start = arr;
  int * end = arr + sizeof(arr) / sizeof(int);
  std::ostream_iterator<int> output_iterator(std::cout, "\t");
  std::copy(start, end, output_iterator);
  std::cout << std::endl;
  return ZERO;
}
