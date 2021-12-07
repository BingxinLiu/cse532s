# Acceptor and Connector Studio

## Q1:

Bingxin Liu

## Q2:

```shell
# OUTPUT:
[bingxin.liu@linuxlab007 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
get_handle
handle signal
handle close with SIGNAL_MASK
handle close with ACCEPT_MASK and SIGNAL_MASK
end and close done
Server Stopped
[bingxin.liu@linuxlab007 ACESimpleServer]$
```


## Q3:

```shell
# CLIENT OUTPUT:
[bingxin.liu@linuxlab007 ACESimpleClient]$ ./ACESimpleClient 33321asdfd
33321asdfd
Start connect to 127.0.0.1:8086
id is 0
send[0] 33321asdfd
33321asdfd
Start connect to 127.0.0.1:8086
id is 1
send[1] 33321asdfd
handle signal
handle close with TIMER_MASK and SIGNAL_MASK
handle close with TIMER_MASK and SIGNAL_MASK
[bingxin.liu@linuxlab007 ACESimpleClient]$

# SERVER OUPUT:
[bingxin.liu@linuxlab007 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
get_handle
RECV [0]
Receive new client
Client Counter: 0
send: 0
RECV [[0] 33321asdfd]
RECV [0]
Receive new client
Client Counter: 1
send: 1
RECV [[1] 33321asdfd]
handle signal
handle close with SIGNAL_MASK
handle close with ACCEPT_MASK and SIGNAL_MASK
handle close in reader with READ_MASK
handle close in reader with READ_MASK
end and close done
[bingxin.liu@linuxlab007 ACESimpleServer]$
```

## Q4:

```shell
# CLIENT OUTPUT:
[bingxin.liu@linuxlab007 ACESimpleClient]$ ./ACESimpleClient 33321asdfd
33321asdfd
Start connect to 127.0.0.1:8086
id is 0
send[0] 33321asdfd
33321asdfd
Start connect to 127.0.0.1:8086
id is 1
send[1] 33321asdfd
handle signal
handle close with TIMER_MASK and SIGNAL_MASK
handle close with TIMER_MASK and SIGNAL_MASK
[bingxin.liu@linuxlab007 ACESimpleClient]$

# SERVER OUTPUT
[bingxin.liu@linuxlab007 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
new server read
get_handle
acceptor handle input
ace_sock_stream.get_handle
RECV [0]
Receive new client
Client Counter: 0
send: 0
RECV [[0] 33321asdfd]
The socket is closed.
ace_sock_stream.get_handle
handle close in reader
handle close in reader with READ_MASK
acceptor handle input
ace_sock_stream.get_handle
RECV [0]
Receive new client
Client Counter: 1
send: 1
RECV [[1] 33321asdfd]
The socket is closed.
ace_sock_stream.get_handle
handle close in reader
handle close in reader with READ_MASK
handle signal
handle close with SIGNAL_MASK
handle close with ACCEPT_MASK and SIGNAL_MASK
end and close done
release server read
Server Stopped
release server acceptor
[bingxin.liu@linuxlab007 ACESimpleServer]$
```

## Q5:

We will receive segmentation fault (core dumped) if we run a client.

```shell
# SERVER OUTPUT:
[bingxin.liu@linuxlab007 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
new server read
acceptor get handle
handle signal
acceptor end_reactor_event_loop()
handle close with SIGNAL_MASK
handle close with ACCEPT_MASK and SIGNAL_MASK
end and close done
Server Stopped
release server acceptor
[bingxin.liu@linuxlab007 ACESimpleServer]$
```

## Q6:

We will receive segmentation fault (core dumped) if we run a client. Before we made changes, cuase we are trying to delete an object which is not dynamically allocated.
Now we dynamically allocate the object and release it when it is no longer needed via handle_close function

```shell
# CLIENT OUTPUT:
[bingxin.liu@linuxlab007 ACESimpleClient]$ ./ACESimpleClient 33321asdfd
33321asdfd
Start connect to 127.0.0.1:8086
id is 0
send[0] 33321asdfd
33321asdfd
Start connect to 127.0.0.1:8086
id is 1
send[1] 33321asdfd
handle signal
handle close with TIMER_MASK and SIGNAL_MASK
handle close with TIMER_MASK and SIGNAL_MASK
[bingxin.liu@linuxlab007 ACESimpleClient]$

# SERVER OUTPUT:
[bingxin.liu@linuxlab007 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
new server read
acceptor get handle
acceptor handle input
new server read
ace_sock_stream.get_handle
handle accept done
RECV [0]
Receive new client
Client Counter: 0
send: 0
RECV [[0] 33321asdfd]
The socket is closed.
ace_sock_stream.get_handle
handle close in reader
reader remove self
handle close in reader with READ_MASKtry to delete this
release server read and sock stream for reader 0

acceptor handle input
new server read
ace_sock_stream.get_handle
handle accept done
RECV [0]
Receive new client
Client Counter: 1
send: 1
RECV [[1] 33321asdfd]
The socket is closed.
ace_sock_stream.get_handle
handle close in reader
reader remove self
handle close in reader with READ_MASKtry to delete this
release server read and sock stream for reader 1

handle signal
acceptor end_reactor_event_loop()
handle close with SIGNAL_MASK
handle close with ACCEPT_MASK and SIGNAL_MASK
end and close done
Server Stopped
release server acceptor
[bingxin.liu@linuxlab007 ACESimpleServer]$
```