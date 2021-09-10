# Studio1

## Q1:

Name: Bingxin Liu

## Q2:

```shell
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ which g++
/project/compute/compilers/gcc-8.3.0/bin/g++
```

## Q3:

```shell
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ pwd
/home/warehouse/bingxin.liu/cse532s/studio1_wrapper_facade
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ ls
a.out  wrapper_facade.cpp
```

## Q4:

```shell
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ g++ -Wall -std=c++11 -pthread wrapper_facade.cpp
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ ./a.out
This is a message from function print_a_single_line_message().

[bingxin.liu@linuxlab002 studio1_wrapper_facade]$
```

## Q5:

```c++
#include <iostream>
#include <thread>

#define ZERO 0

void print_a_single_line_message();

int
main (int, char * [])
{
  std::thread t(print_a_single_line_message);
  t.join();
  return ZERO;
}

void
print_a_single_line_message ()
{
  std::cout << "This is a message from function print_a_single_line_message().\n" << std::endl;
}
```

## Q6:

```shell
# The outputs of multithreads:
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ g++ -Wall -std=c++11 -pthread wrapper_facade.cpp
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ ./a.out
This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

This is a message from function print_a_single_line_message().

[bingxin.liu@linuxlab002 studio1_wrapper_facade]$
```

```c++
//	The code of multithreads
#include <iostream>
#include <thread>

#define ZERO 0

void print_a_single_line_message();

int
main (int, char * [])
{
  for (int i = 0; i < 10; ++i)
  {
      std::thread t(print_a_single_line_message);
      t.join();
  }
  return ZERO;
}

void
print_a_single_line_message ()
{
  std::cout << "This is a message from function print_a_single_line_message().\n" << std::endl;
}
```

## Q7:

```shell
#	The outputs:
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ g++ -Wall -std=c++11 -pthread wrapper_facade.cpp
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$ ./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
./a.out 1 2 3 4 5 6 7 8 9
[bingxin.liu@linuxlab002 studio1_wrapper_facade]$
```

```c++
//	The code
File Edit Options Buffers Tools C++ Help
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
```

