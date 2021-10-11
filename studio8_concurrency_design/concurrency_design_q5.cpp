#include <iostream>
#include <cstdlib>
#include <time.h>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <sys/time.h>

#define ZERO 0

const int rows = 4;
const int cols = 4;

std::mutex cout_mutex;

template<class T>
void print(T data)
{
    //std::lock_guard<std::mutex> lock(cout_mutex);
    //std::cout << data << std::endl;
}
void matrix_add(int A[][cols], int B[][cols], int C[][cols])
{
    int const matrix_size = rows*cols;
    int const cacheline_size = 64;
    int const rectangle_region_size = cacheline_size / sizeof(int);

    int* a_start = *A;
    int* a_last = a_start + matrix_size;
    int* b_start = *B;
    int* c_start = *C;

    int const hardware_threads = std::thread::hardware_concurrency();
    int const block_size = matrix_size / hardware_threads;

    int* a_block_start = a_start;
    int* b_block_start = b_start;
    int* c_block_start = c_start;
    std::vector<std::thread> threads(hardware_threads - 1);
    for (int i = 0; i < hardware_threads - 1; ++i)
    {
        int* a_block_end = a_block_start + block_size;
        int* b_block_end = b_block_start + block_size;
        int* c_block_end = c_block_start + block_size;
        threads.push_back(std::thread([=](){
            int * a_it = a_block_start;
            int * b_it = b_block_start;
            int * c_it = c_block_start;
            for (;a_it != a_block_end;)
            {
                *c_it = *a_it + *b_it;
                a_it++;
                b_it++;
                c_it++;
                
            }
        }));
        a_block_start = a_block_end;
        b_block_start = b_block_end;
        c_block_start = c_block_end;
    }
    for (;a_block_start != a_last;)
    {
        *c_block_start = *a_block_start + *b_block_start;
        a_block_start++;
        b_block_start++;
        c_block_start++;
    }
    for(size_t i = 0; i < threads.size(); ++i)
        if(threads[i].joinable())
            threads[i].join();

}

int
main(int, char * [])
{
    int A[rows][cols];
    int B[rows][cols];
    int C[rows][cols];

    srand((unsigned) time(NULL));
    std::cout << "A = [" << std::endl;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            A[i][j] = rand() % 100;
            std::cout << "(" << i << "," << j << "):" << A[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;

    //srand((unsigned) time(NULL));
    std::cout << "B = [" << std::endl;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            B[i][j] = rand() % 100;
            std::cout << "(" << i << "," << j << "):" << B[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;

    struct timeval t1,t2;
    double timeuse;

    gettimeofday(&t1, NULL);
    matrix_add(A, B, C);
    gettimeofday(&t2, NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    std::cout << "variation1 time = " << timeuse << " seconds" <<std::endl;

    std::cout << "C = [" << std::endl;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            std::cout << "(" << i << "," << j << "):" << C[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;
    


}