#include <iostream>
#include <thread>

#define ZERO 0

void print_a_single_line_message(int, char **);

int
main (int argc, char * argv[])
{
  for (int i = 0; i < 10; ++i)
  {
    std::thread t(print_a_single_line_message, argc, argv);
      t.join();
  }
  return ZERO;
}

void
print_a_single_line_message (int argc, char * argv[])
{
  for (int i = 0; i < argc; ++i)
    {
      std::cout << argv[i] << " ";
    }
  std::cout << std::endl;
}
