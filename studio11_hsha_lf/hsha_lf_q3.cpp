#include <iostream>
#include "thread_pool.hpp"

#define SUCCESS 0

using namespace std;

bool
is_prime(unsigned int n)
{
    if ( n == 0 || n == 1 ) return false;
    for ( unsigned int i = 2; i <= n / 2; ++i )
    {
        if ( n % i == 0 ) return false;
    }
    return true;
}


int 
main(int argc, char* argv[])
{
    thread_pool thread_pool;

    thread_pool.submit(move(make_pair(&is_prime, 5)));
    thread_pool.submit(move(make_pair(&is_prime, 12)));
    thread_pool.submit(move(make_pair(&is_prime, 97)));
    thread_pool.submit(move(make_pair(&is_prime, 32)));
    thread_pool.submit(move(make_pair(&is_prime, 37)));
    thread_pool.submit(move(make_pair(&is_prime, 61)));
    thread_pool.submit(move(make_pair(&is_prime, 7)));

    return SUCCESS;
}