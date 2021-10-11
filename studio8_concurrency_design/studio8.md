# STUDIO 8 : Concurrency Design Studio

## Q1:

Bingxin Liu

## Q2:

```shell
# OUTPUT:
[bingxin.liu@linuxlab008 studio8_concurrency_design]$ ./a.out
A = [
(0,0):0x7ffe587b88f0	(0,1):0x7ffe587b88f4	(0,2):0x7ffe587b88f8	(0,3):0x7ffe587b88fc	(0,4):0x7ffe587b8900	(0,5):0x7ffe587b8904	(0,6):0x7ffe587b8908	(0,7):0x7ffe587b890c	(0,8):0x7ffe587b8910	(0,9):0x7ffe587b8914	(0,10):0x7ffe587b8918	(0,11):0x7ffe587b891c	(0,12):0x7ffe587b8920	(0,13):0x7ffe587b8924	(0,14):0x7ffe587b8928	(0,15):0x7ffe587b892c
(1,0):0x7ffe587b8930	(1,1):0x7ffe587b8934	(1,2):0x7ffe587b8938	(1,3):0x7ffe587b893c	(1,4):0x7ffe587b8940	(1,5):0x7ffe587b8944	(1,6):0x7ffe587b8948	(1,7):0x7ffe587b894c	(1,8):0x7ffe587b8950	(1,9):0x7ffe587b8954	(1,10):0x7ffe587b8958	(1,11):0x7ffe587b895c	(1,12):0x7ffe587b8960	(1,13):0x7ffe587b8964	(1,14):0x7ffe587b8968	(1,15):0x7ffe587b896c
(2,0):0x7ffe587b8970	(2,1):0x7ffe587b8974	(2,2):0x7ffe587b8978	(2,3):0x7ffe587b897c	(2,4):0x7ffe587b8980	(2,5):0x7ffe587b8984	(2,6):0x7ffe587b8988	(2,7):0x7ffe587b898c	(2,8):0x7ffe587b8990	(2,9):0x7ffe587b8994	(2,10):0x7ffe587b8998	(2,11):0x7ffe587b899c	(2,12):0x7ffe587b89a0	(2,13):0x7ffe587b89a4	(2,14):0x7ffe587b89a8	(2,15):0x7ffe587b89ac
(3,0):0x7ffe587b89b0	(3,1):0x7ffe587b89b4	(3,2):0x7ffe587b89b8	(3,3):0x7ffe587b89bc	(3,4):0x7ffe587b89c0	(3,5):0x7ffe587b89c4	(3,6):0x7ffe587b89c8	(3,7):0x7ffe587b89cc	(3,8):0x7ffe587b89d0	(3,9):0x7ffe587b89d4	(3,10):0x7ffe587b89d8	(3,11):0x7ffe587b89dc	(3,12):0x7ffe587b89e0	(3,13):0x7ffe587b89e4	(3,14):0x7ffe587b89e8	(3,15):0x7ffe587b89ec
]
B = [
(0,0):0x7ffe587b87f0	(0,1):0x7ffe587b87f4	(0,2):0x7ffe587b87f8	(0,3):0x7ffe587b87fc
(1,0):0x7ffe587b8800	(1,1):0x7ffe587b8804	(1,2):0x7ffe587b8808	(1,3):0x7ffe587b880c
(2,0):0x7ffe587b8810	(2,1):0x7ffe587b8814	(2,2):0x7ffe587b8818	(2,3):0x7ffe587b881c
(3,0):0x7ffe587b8820	(3,1):0x7ffe587b8824	(3,2):0x7ffe587b8828	(3,3):0x7ffe587b882c
(4,0):0x7ffe587b8830	(4,1):0x7ffe587b8834	(4,2):0x7ffe587b8838	(4,3):0x7ffe587b883c
(5,0):0x7ffe587b8840	(5,1):0x7ffe587b8844	(5,2):0x7ffe587b8848	(5,3):0x7ffe587b884c
(6,0):0x7ffe587b8850	(6,1):0x7ffe587b8854	(6,2):0x7ffe587b8858	(6,3):0x7ffe587b885c
(7,0):0x7ffe587b8860	(7,1):0x7ffe587b8864	(7,2):0x7ffe587b8868	(7,3):0x7ffe587b886c
(8,0):0x7ffe587b8870	(8,1):0x7ffe587b8874	(8,2):0x7ffe587b8878	(8,3):0x7ffe587b887c
(9,0):0x7ffe587b8880	(9,1):0x7ffe587b8884	(9,2):0x7ffe587b8888	(9,3):0x7ffe587b888c
(10,0):0x7ffe587b8890	(10,1):0x7ffe587b8894	(10,2):0x7ffe587b8898	(10,3):0x7ffe587b889c
(11,0):0x7ffe587b88a0	(11,1):0x7ffe587b88a4	(11,2):0x7ffe587b88a8	(11,3):0x7ffe587b88ac
(12,0):0x7ffe587b88b0	(12,1):0x7ffe587b88b4	(12,2):0x7ffe587b88b8	(12,3):0x7ffe587b88bc
(13,0):0x7ffe587b88c0	(13,1):0x7ffe587b88c4	(13,2):0x7ffe587b88c8	(13,3):0x7ffe587b88cc
(14,0):0x7ffe587b88d0	(14,1):0x7ffe587b88d4	(14,2):0x7ffe587b88d8	(14,3):0x7ffe587b88dc
(15,0):0x7ffe587b88e0	(15,1):0x7ffe587b88e4	(15,2):0x7ffe587b88e8	(15,3):0x7ffe587b88ec
]
C = [
(0,0):0x7ffe587b86f0	(0,1):0x7ffe587b86f4	(0,2):0x7ffe587b86f8	(0,3):0x7ffe587b86fc	(0,4):0x7ffe587b8700	(0,5):0x7ffe587b8704	(0,6):0x7ffe587b8708	(0,7):0x7ffe587b870c
(1,0):0x7ffe587b8710	(1,1):0x7ffe587b8714	(1,2):0x7ffe587b8718	(1,3):0x7ffe587b871c	(1,4):0x7ffe587b8720	(1,5):0x7ffe587b8724	(1,6):0x7ffe587b8728	(1,7):0x7ffe587b872c
(2,0):0x7ffe587b8730	(2,1):0x7ffe587b8734	(2,2):0x7ffe587b8738	(2,3):0x7ffe587b873c	(2,4):0x7ffe587b8740	(2,5):0x7ffe587b8744	(2,6):0x7ffe587b8748	(2,7):0x7ffe587b874c
(3,0):0x7ffe587b8750	(3,1):0x7ffe587b8754	(3,2):0x7ffe587b8758	(3,3):0x7ffe587b875c	(3,4):0x7ffe587b8760	(3,5):0x7ffe587b8764	(3,6):0x7ffe587b8768	(3,7):0x7ffe587b876c
(4,0):0x7ffe587b8770	(4,1):0x7ffe587b8774	(4,2):0x7ffe587b8778	(4,3):0x7ffe587b877c	(4,4):0x7ffe587b8780	(4,5):0x7ffe587b8784	(4,6):0x7ffe587b8788	(4,7):0x7ffe587b878c
(5,0):0x7ffe587b8790	(5,1):0x7ffe587b8794	(5,2):0x7ffe587b8798	(5,3):0x7ffe587b879c	(5,4):0x7ffe587b87a0	(5,5):0x7ffe587b87a4	(5,6):0x7ffe587b87a8	(5,7):0x7ffe587b87ac
(6,0):0x7ffe587b87b0	(6,1):0x7ffe587b87b4	(6,2):0x7ffe587b87b8	(6,3):0x7ffe587b87bc	(6,4):0x7ffe587b87c0	(6,5):0x7ffe587b87c4	(6,6):0x7ffe587b87c8	(6,7):0x7ffe587b87cc
(7,0):0x7ffe587b87d0	(7,1):0x7ffe587b87d4	(7,2):0x7ffe587b87d8	(7,3):0x7ffe587b87dc	(7,4):0x7ffe587b87e0	(7,5):0x7ffe587b87e4	(7,6):0x7ffe587b87e8	(7,7):0x7ffe587b87ec
]
[bingxin.liu@linuxlab008 studio8_concurrency_design]$ cat /sys/devices/system/cpu/cpu1/cache/index0/coherency_line_size
64
[bingxin.liu@linuxlab008 studio8_concurrency_design]$
```

From above, we can find that the size of cache line is 64 bytes, which means for the first array, we can make each row as a rectangular region, for the second one, we can make each continuous four rows as a region, and for the last one, each continuous two rows could work.

I don't think there is a single common formula for defining contiguous rectangular regions, because the format of each data blocks is different. Therefore, we have to examine the actually lay out of data blocks, and then decide the rectangular regions.



## Q3:

1. 32 threads are used in each variation

2. Code:

    ```c++
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
    
    // IN mian function
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
    ```

3. Output

```shell
hardware_threads=32
variation1 time = 0.07557 seconds
hardware_threads=32
variation2 time = 0.001749 seconds
[bingxin.liu@linuxlab007 studio8_concurrency_design]$
```

The variation2 is faster than variation1, which is surprising. This may caused by the size of our array. Our array only has 16 * 4 items, so the data contention is not as much as we expected. Also, the variation 2 is actually spawn less threads than variation 1, which reduce the overhead of launching a thread.

## Q4:

```shell
# OUTPUT without print
[bingxin.liu@linuxlab009 studio8_concurrency_design]$ ./a.out
variation1 time = 0.001726 seconds
# 4*4

[bingxin.liu@linuxlab009 studio8_concurrency_design]$ ./a.out
variation1 time = 0.001833 seconds
# 16*16

[bingxin.liu@linuxlab009 studio8_concurrency_design]$ ./a.out
variation1 time = 0.00304 seconds
# 1024*1024
```

In this part, we comment out the code that prints A, B, and C, only record the time of running matrix addition function. As we can see, when the matrix grown from 4\*4 to 16\*16, the performance isn't improved significantly. This is because each thread only handle with a few items. The overhead of launching a thread is much larger than doing the calculation. As a consequence, after the dimension of our matrix increased to 1024, the improvement is good and it also proves that we have a good scalability.

```shell
# OUTPUT with print
[bingxin.liu@linuxlab009 studio8_concurrency_design]$ ./a.out
A = [
(0,0):86	(0,1):58	(0,2):12	(0,3):81
(1,0):72	(1,1):7	(1,2):95	(1,3):85
(2,0):19	(2,1):47	(2,2):85	(2,3):71
(3,0):83	(3,1):47	(3,2):68	(3,3):53
]
B = [
(0,0):78	(0,1):0	(0,2):40	(0,3):66
(1,0):27	(1,1):95	(1,2):31	(1,3):13
(2,0):77	(2,1):25	(2,2):6	(2,3):24
(3,0):27	(3,1):52	(3,2):24	(3,3):65
]
variation1 time = 0.001853 seconds
C = [
(0,0):164	(0,1):58	(0,2):52	(0,3):147
(1,0):99	(1,1):102	(1,2):126	(1,3):98
(2,0):96	(2,1):72	(2,2):91	(2,3):95
(3,0):110	(3,1):99	(3,2):92	(3,3):118
]
[bingxin.liu@linuxlab009 studio8_concurrency_design]$
```

## Q5:

```shell
A = [
56	28	93	92
43	32	54	9
63	23	41	32
83	12	43	45
]

B = [
19	45	82	92
99	28	54	38
26	87	37	21
39	92	18	31
]

C = [
9842	19859	11201	11021
5740	8357	7414	6585
5788	9990	8501	8523
5638	11952	9855	10390
]
[bingxin.liu@linuxlab009 studio8_concurrency_design]$
```

In this program, if we can get the transformation of matrix B before we start to comput the result, the running time of the program will have a huge improvement, which is due to the improvement of locality. Therefore, less cache missing happens and performance improved.