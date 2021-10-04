# STUDIO 5 : Active Object Studio

## Q1:

Bingxin Liu

## Q2:

If we manage to make `enqueue()` matches with `run_service()`, the program runs correctly, which means the program will not try to `run_service()` when there is no items could be popped to be sericed. Otherwise, the program will be deadlock since the program try to `run_service()` in an empty queue, but it can not preempt itself to `enqueue()` an item.

## Q3:

Now, the program is no longer blocked, even it try to pop an item from an empty queue. Therefore, the action of the program becomes normal. However, the thread running `run_service` will run forever since no one notice it that it should stop itself.



## Q4:

Now, the thread running `run_service` could stop normally.

```shell
# RESULT:
[bingxin.liu@linuxlab006 studio5_active_object]$ g++ -Wall -std=c++11 -pthread ./active_object_q4.cpp
[bingxin.liu@linuxlab006 studio5_active_object]$ ./a.out
THREAD 47493108602624 : run_service started.
push front: 0
push front: 1
push front: 2
push front: 3
push front: 4
pop back: 0
run_service() get: 0
push front: 5
pop back: 1
run_service() get: 1
pop back: 2
run_service() get: 2
pop back: 3
run_service() get: 3
pop back: 4
run_service() get: 4
pop back: 5
run_service() get: 5
THREAD 47493108602624 : run_service stopped.
```

## Q5:

```shell
# OUTPUT:
[bingxin.liu@linuxlab006 studio5_active_object]$ ./a.out
THREAD 47653717890816 : run_service started.
push front: 0
push front: 1
push front: 2
push front: 3
push front: 4
pop back: 0
run_service() get: 0
push front: 5
push front: 0
pop back: 1
run_service() get: 1
push front: 1
pop back: 2
run_service() get: 2
push front: 2
pop back: 3
run_service() get: 3
push front: 3
pop back: 4
run_service() get: 4
push front: 4
pop back: 5
run_service() get: 5
THREAD 47653717890816 : run_service stopped.
```

According to the output, we can find that each time second active object pops an item, it will push that item into the first one.