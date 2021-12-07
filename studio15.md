# Interceptor Studio

## Q1:

Bingxin Liu

## Q2:

```c++
class interceptor
{
public:
    virtual void operator()(char&)=0;
};
```

## Q3:

```c++
// add a class member in the read event handler
interceptor* server_interceptor;

// add a public member function so that accept event handler can set the interceptor
void set_interceptor(interceptor* intercptr)
{
    server_interceptor = intercptr;
}

// dynamically allocate read event handler and a pass through interceptor
server_read* reader = new server_read;
pass_through_interceptor* intercptr = new pass_through_interceptor;

// set the interceptor class member
reader->set_interceptor(intercptr);

// In the handle_input function of read event handler, use the function operator to operate the receive message
if ( this->server_interceptor != nullptr )
{
    transform(recv_str.begin(), recv_str.end(), recv_str.begin(), [this](char c) -> char{
        this->server_interceptor->operator()(c);
        return c;
    });
    cout << endl;
}
```

The client side is basically the same thing except that we don't dynamically allocate it.

## Q4:

The client side right rotate the character 2 bits and the server side modulo add the character with 2.
The output like "[\**|**]" tells us the original/received character in Hexadecimal in the LHS, and the character operated after passing through the interceptor.

```shell
# CLIENT OUTPUT:
[bingxin.liu@linuxlab005 ACESimpleClient]$ ./ACESimpleClient dsb123
dsb123
Start connect to 127.0.0.1:8086
id is 0
send length = 11
[5b|d6][30|c][5d|57][20|8][64|19][73|dc][62|98][31|4c][32|8c][33|cc][a|82]send�ܘL�̂

...

[bingxin.liu@linuxlab005 ACESimpleClient]$


# SERVER OUTPUT:
[bingxin.liu@linuxlab005 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
Start listening on 127.0.0.1:8086
Receive new client
Client Counter: 0
receive 11[�ܘL�̂]
[d6|d8][c|e][57|59][8|a][19|1b][dc|de][98|9a][4c|4e][8c|8e][cc|ce]
RECV [�YޚN�΂]
The socket is closed.

...

[bingxin.liu@linuxlab005 ACESimpleServer]$
```

## Q5:

I changed the data structure of the root class and declare a new virtual function like:

```c++
class interceptor
{
public:
    virtual void operator()(char&)=0;
    virtual void operator()(char&, int)=0;
};
```

So that each concrete interceptor's function operator will have a second parameter, which indicate the hint of how should we operate the character. In the source file, I implement serval different conctrete interceptor, which are showed below:

```c++
void
mod_interceptor::operator()(char& c, int size)
{
    // char res = c;
    // res = res - size;
    std::cout << "[" << std::hex << (0xFF & c) << "|";
    unsigned char res = ((unsigned char)c);
    res = ((int) res) + size;
    std::cout << std::hex << (0xFF & res) << "]";
    
    c = (char) res;
}
void
rotation_interceptor::operator()(char& c, int size)
{
    // char res = c;
    // res = res - size;
    if ( size > 0 )
    {
        std::cout << "[" << std::hex << (0xFF & c) << "|";
        unsigned char res = ((unsigned char)c) >> size;
        res |= ((unsigned char)c) << (8 - size);
        std::cout << std::hex << (0xFF & res) << "]";
        c = (char) res;
    } else if ( size < 0 ) 
    {
        size = -size;
        std::cout << "[" << std::hex << (0xFF & c) << "|";
        unsigned char res = ((unsigned char)c) << size;
        res |= ((unsigned char)c) >> (8 - size);
        std::cout << std::hex << (0xFF & res) << "]";
        
        c = (char) res;
    } else 
    {
        return;
    }
}
```

## Q6:

I used chain of responsibility pattern in this exercise.
First I declare a new class name composer, which has two member functions. One is compose() and the other one is execute(). 

```c++
class interceptor_composer
{
    vector<function<...>*> chain;
public:
    void compose(interceptor* intercptr, int size);
    void execute(char& c);

};
```

The compose function will receive a pointer of interceptor and its operator value, the function will bind the second argument of the interceptor's function operator with the value, which will return a function pointer and we push it into a vector.
The execute function is fair simple, it just operate the input character by those functions in the vector one by one.

In this exercise, I tried to first add 2 and then right rotate the charactor 2 bits on the client side, and reverse operations on the server side.

The output shows below:

```shell
# CLIENT SIDE:
[bingxin.liu@linuxlab005 ACESimpleClient]$ ./ACESimpleClient dsb123
...
send length = 11
[5b|5d][5d|57][30|32][32|8c][5d|5f][5f|d7][20|22][22|88][64|66][66|99][73|75][75|5d]M62|64][64|19][31|33][33|cc][32|34][34|d][33|35][35|4d][a|c][c|3]sendW�׈�]�
...
[bingxin.liu@linuxlab005 ACESimpleClient]$

# SERVER SIDE:
[bingxin.liu@linuxlab005 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
...
MO�]ive 11[W�׈�]�
[5d|5b][57|5d][5d|5b][8c|32][32|30][d7|5f][5f|5d][88|22][22|20][99|66][66|64][5d|75][75|73][19|64][64|62][cc|33][33|31][d|34][34|32][4d|35][35|33][3|c]
RECV [[0] dsb123]
...
[bingxin.liu@linuxlab005 ACESimpleServer]$
```

## Q7:

The module rotation and module add/sub operation is revertible, which are shown in Q6. However, some operations like multiple and divide by a const value is not revertible. The output shows below.

```shell
# CLIENT OUTPUT
[bingxin.liu@linuxlab005 ACESimpleClient]$ ./ACESimpleClient dsb123
...
send length = 11
[5b|5e][5e|bc][30|33][33|66][5d|60][60|c0][20|23][23|46][64|67][67|ce][73|76][76|ec][62|65][65|ca][31|34][34|68][32|35][35|6a][33|36][36|6c][a|d][d|1a]send�f�F���hjl
...
[bingxin.liu@linuxlab005 ACESimpleClient]$

# SERVER OUTPUT
[bingxin.liu@linuxlab005 ACESimpleServer]$ ./ACESimpleServer
./ACESimpleServer
...
receive 11[�f�F���hjl��]
[5b|5e][bc|de][de|db][66|33][33|30][c0|e0][e0|dd][46|23][23|20][ce|e7][e7|e4][ec|f6][f6|f3][ca|e5][e5|e2][68|34][34|31][6a|35][35|32][6c|36][36|33][1a|d]
RECV [�0� ���123]
...
[bingxin.liu@linuxlab005 ACESimpleServer]$
```

As we can see, some characters could be reverted but some others cannot. This is because the divide operation in computer is not accurate.