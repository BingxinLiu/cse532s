# STUDIO 6 : Atomic Types Studio

## Q1:

Bingxin Liu

## Q2:

```c++
    std::atomic<bool>
    std::atomic<char>
    std::atomic<signed char>
    std::atomic<unsigned char>
    std::atomic<int>
    std::atomic<unsigned>
    std::atomic<short>
    std::atomic<unsigned short>
    std::atomic<long>
    std::atomic<unsigned long>
    std::atomic<long long>
    std::atomic<unsigned long long>
    std::atomic<char16_t>
    std::atomic<char32_t>
    std::atomic<wchar_t>
    atomic_int_least8_t
    atomic_uint_least8_t
    atomic_int_least16_t
    atomic_uint_least16_t
    atomic_int_least32_t
    atomic_uint_least32_t
    atomic_int_least64_t
    atomic_uint_least64_t
    atomic_int_fast8_t
    atomic_uint_fast8_t
    atomic_int_fast16_t
    atomic_uint_fast16_t
    atomic_int_fast32_t
    atomic_uint_fast32_t
    atomic_int_fast64_t
    atomic_uint_fast64_t
    atomic_intptr_t
    atomic_uintptr_t
    atomic_size_t
    atomic_ptrdiff_t
    atomic_intmax_t
    atomic_uintmax_t
```

## Q3:

```c++
// is lock free
		std::atomic<bool>
    std::atomic<char>
    std::atomic<signed char>
    std::atomic<unsigned char>
    std::atomic<int>
    std::atomic<unsigned>
    std::atomic<short>
    std::atomic<unsigned short>
    std::atomic<long>
    std::atomic<unsigned long>
    std::atomic<long long>
    std::atomic<unsigned long long>
    std::atomic<char16_t>
    std::atomic<char32_t>
    std::atomic<wchar_t>
    atomic_int_least8_t
    atomic_uint_least8_t
    atomic_int_least16_t
    atomic_uint_least16_t
    atomic_int_least32_t
    atomic_uint_least32_t
    atomic_int_least64_t
    atomic_uint_least64_t
    atomic_int_fast8_t
    atomic_uint_fast8_t
    atomic_int_fast16_t
    atomic_uint_fast16_t
    atomic_int_fast32_t
    atomic_uint_fast32_t
    atomic_int_fast64_t
    atomic_uint_fast64_t
    atomic_intptr_t
    atomic_uintptr_t
    atomic_size_t
    atomic_ptrdiff_t
    atomic_intmax_t
    atomic_uintmax_t
```

Actually, all of atomic_types listed in tables 5.1 and 5.2 are lock free.

## Q4:

```shell
# OUTPUT:
[bingxin.liu@linuxlab009 studio6_atomic_types]$ ./a.out 10000000
z = 10000000
```

As we can see, the number of iteration is 10000000, but the output is still 10000000. This tell us even the out-of-order may happens, but in some case, even in most case, it will not happen.



## Q5:

```shell
# OUTPUT:
[bingxin.liu@linuxlab004 studio6_atomic_types]$ ./a.out 1000000
z = 1000000
[bingxin.liu@linuxlab004 studio6_atomic_types]$
```

The behaviour isn't changed. By using memory_order_seq_cst, we have a strong sequential consistent ordering, we will not expect any out-of-order happen. However, the behavior is not changed only because the out-of-order didn't happen in Q4. If it happenned in Q4, the behavior will be different.



## Q6:

```c++
void write_x_then_y()
{
    x.store(true,std::memory_order_relaxed);
    y.store(true,std::memory_order_release);
}

void read_y_then_x()
{
    while(!y.load(std::memory_order_acquire));
    if(x.load(std::memory_order_relaxed))
        ++z;
}
```

```shell
[bingxin.liu@linuxlab009 studio6_atomic_types]$ ./a.out 10000
z = 10000
[bingxin.liu@linuxlab009 studio6_atomic_types]$
```

Q5 showed program behavior that was most similar to that seen in this exercise.

This is because in this exercise, the x.store happens before y.store, y.store happens before y.load, and y.load happens before x.load. Therefore, we have x.store happens before x.load, which means the `++z` will be triggered in each time.