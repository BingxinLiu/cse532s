#include <iostream>
#include <cstdlib>
#include <time.h>

#define ZERO 0

int
main(int, char * [])
{
    int A[4][16];
    int B[16][4];
    int C[8][8];

    srand((unsigned) time(NULL));
    std::cout << "A = [" << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            A[i][j] = rand();
            std::cout << "(" << i << "," << j << "):" << &A[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;

    srand((unsigned) time(NULL));
    std::cout << "B = [" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            B[i][j] = rand();
            std::cout << "(" << i << "," << j << "):" << &B[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;

    srand((unsigned) time(NULL));
    std::cout << "C = [" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            C[i][j] = rand();
            std::cout << "(" << i << "," << j << "):" << &C[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;

}