#include <iostream>

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
    cout << " 5 " << (is_prime(5) ? "is a prime number" : "is not a prime number") << endl; 
    cout << " 51 " << (is_prime(51) ? "is a prime number" : "is not a prime number") << endl;
    cout << " 23 " << (is_prime(23) ? "is a prime number" : "is not a prime number") << endl;
    cout << " 86 " << (is_prime(86) ? "is a prime number" : "is not a prime number") << endl;

    return SUCCESS;
}