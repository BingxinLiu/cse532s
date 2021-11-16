#include <iostream>
#include <string>
#include <fstream>
#include "thread_pool.hpp"

#define SUCCESS 0

using namespace std;

bool
is_prime(long long n)
{
    if ( n == 0 || n == 1 ) return false;
    for ( long long i = 2; i <= n / 2; ++i )
    {
        if ( n % i == 0 ) return false;
    }
    return true;
}


int 
main(int argc, char* argv[])
{
    bool interactive = false;

    if (argc < 2 || argc > 3)
    {
        std::cout << "USAGE: ./hsha_If_q5 {-i|-f} [-f <filename>]" << endl;
        return EINVAL;
    }
    
    string config_arg(argv[1]);
    string input_file;
    string input_str;
    long long input_number;

    if (config_arg == string("-i")) interactive = true;
    else if (config_arg != string("-f") || argc != 3)
    {
        std::cout << "USAGE: ./hsha_If_q5 {-i|-f} [-f <filename>]" << endl;
        return EINVAL;
    } else
    {
        input_file = string(argv[2]);
    }

    thread_pool thread_pool;

    if (interactive)
    {
        std::cout << "Please input numbers" << endl;
        for(;;)
        {
            std::cin >> input_str;
            try
            {
                input_number = stoll(input_str);
            } catch(...)
            {
                if (input_str == "Q") break;
                std::cout << "invalid input" << input_str << endl;
                continue;
            }
            thread_pool.submit(move(make_pair(&is_prime, input_number)));

        }
    } else
    {
        string line;
        ifstream myfile (input_file);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                try
                {
                    input_number = stoll(line);
                } catch(...)
                {
                    if (line == "Q") break;
                    std::cout << "invalid input" << line << endl;
                    continue;
                }
                thread_pool.submit(move(make_pair(&is_prime, input_number)));
                
            }
            myfile.close();
        }
        else std::cout << "Unable to open file"; 

    }


    return SUCCESS;
}