#include <iostream>
#include <cstdlib>
#include <time.h>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <sys/time.h>

#define ZERO 0

std::mutex cout_mutex;
int A[4][16];
int B[16][4];
int C[8][8];

template<class T>
void print(T data)
{
    //std::lock_guard<std::mutex> lock(cout_mutex);
    //std::cout << data << std::endl;
}

void variation2(std::vector<int> target_array, int array_length)
{
    unsigned long const hardware_threads=
        std::thread::hardware_concurrency();

    std::cout << "hardware_threads=" << hardware_threads << std::endl;

    unsigned long const num_threads=
        hardware_threads >= 2 ? hardware_threads : 4;

    for (std::vector<int>::iterator start = target_array.begin();
            start != target_array.end();)
    {
        unsigned long const tmp=std::distance(start, target_array.end());
        int patch_number = std::min(num_threads, tmp);

        std::vector<std::thread> threads(num_threads-1);
        for (int i = 0; i < patch_number - 1; ++i)
        {
            int target = *start;
            print(target);
            threads[i] = std::thread([=](){
                for (int* it = *B;
                    it != *(B+array_length);
                    ++it)
                {
                    if (*it == target) 
                    {
                        print(*it);
                        print(&*it);
                    }
                }
            });
            start++;
        }
        int target = *start;
        for (int* it = *B;
                    it != *(B+array_length);
                    ++it)
        {
            if (*it == target)
            {
                print(*it);
                print(&*it);
            }
        }
        start++;
        for(int i=0;i<patch_number - 1;++i)
        {
            threads[i].join();
        }
    }
    

}

void variation1(std::vector<int> target_array, int array_length)
{
    unsigned long const length = array_length;
    unsigned long const hardware_threads=
        std::thread::hardware_concurrency();

    std::cout << "hardware_threads=" << hardware_threads << std::endl;

    unsigned long const num_threads=
        hardware_threads >= 2 ? hardware_threads : 4;

    unsigned long const block_size=length/num_threads;

    for (long unsigned int i = 0; i < target_array.size(); ++i)
    {
        int target = target_array[i];

        std::vector<std::thread> threads(num_threads-1);
        {
            int * start = *B; 
            int * last = *(B + length);
            
            for (unsigned long i = 0; i < (num_threads-1); ++i)
            {
                int * end = start + block_size;
                threads[i] = std::thread([=](){
                    int* inner_start = start;
                    for (;inner_start != end; ++inner_start)
                    {
                        if (*inner_start == target)
                        {
                            print(*inner_start);
                            print(&*inner_start);
                        }
                    }
                });
                start = end;
            }
            for(;start != last; ++start)
            {
                if (*start == target)
                {
                    print(*start);
                    print(&*start);
                }
            }
        }
        for(unsigned i=0;i<threads.size();++i)
        {
            threads[i].join();
        }
    }
}

int
main(int, char * [])
{
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

    std::vector<int> target_array;
    for(int i = 0; i <80; ++i)
        target_array.push_back(i);
    struct timeval t1,t2;
    double timeuse;

    gettimeofday(&t1, NULL);
    variation1(target_array, 4*16);
    gettimeofday(&t2, NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    std::cout << "variation1 time = " << timeuse << " seconds" <<std::endl;
    
    gettimeofday(&t1, NULL);
    variation2(target_array, 4*16);
    gettimeofday(&t2, NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    std::cout << "variation2 time = " << timeuse << " seconds" <<std::endl;


}