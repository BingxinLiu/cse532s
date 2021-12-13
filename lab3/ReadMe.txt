=====================================================================================
=========================== CSE532S Fall 2021 LAB3 REPORT ===========================
=====================================================================================

---------------|
1. Team Member |
---------------|
    Bingxin Liu : bingxin.liu@wustl.edu


----------------------|
2. Overview of Design |
----------------------|

Structure of the dirctory:
.lab3
├── ReadMe.txt
├── director
│   ├── Makefile
│   ├── config.hpp
│   ├── director.cpp
│   ├── director.hpp
│   ├── main.cpp
│   ├── play.cpp
│   ├── play.hpp
│   ├── player.cpp
│   └── player.hpp
├── producer
│   ├── Makefile
│   ├── main.cpp
│   ├── producer.cpp
│   ├── producer.hpp
│   ├── reader_service.cpp
│   ├── reader_service.hpp
│   ├── threadsafe_menu.cpp
│   ├── threadsafe_menu.hpp
│   ├── ui_service.cpp
│   └── ui_service.hpp
└── utilities
    ├── const.hpp
    ├── threadsafe_io.cpp
    ├── threadsafe_io.hpp
    ├── utilities.cpp
    └── utilities.hpp


- lab2 files:
    ├── play.cpp
    ├── play.hpp
    ├── player.cpp
    └── player.hpp

- director
    source files for director program
- producer
    source files for producer program
- utilities
    some helper function and const value define

2.0 Logic Overview
In this lab, there are 2 main program: prodeucer and director. Prodeucer has the responsibility to control UI and connect with directors. Director has the duty to register their available plays, and play plays when it has to.

2.1 Design of the Director program
The program will first parse its command line arguments like what we did in lab2, except that the director will also establish a connection with the Producer. After the connection established, the director will send '[director_id] 0' to the producer. Then the producer will return a unique id for this director as its id. Then the director reactively listen from the producer, if the producer send star, stop, quit command, it will reponse respectively.

2.2 Design of the Producer program

The program will frist listen to a specific port. it provide three service class which are inherited from ACE_Event_Handler. The first one is UI service, it will listen on standard input, parse the command, and let the producer send messages to corresponding socket, which will end up with reaching one director. Also, the ui_service will provide a new menu each time the play started/stopped/finished or new director connected with its new supported plays. The second one is producer(class) service, which is similar as an acceptor in Connector/Acceptor mode. It accept a connection request, establish the connection and a reader service, which will be used as a receiver for director's message. The third one is reader service, it is built by the producer whenever it receive and establish a connection with the director. After that, the reader will on behalf of the producer to do all the communication. It will send messages to director and recieve and parse the message coming from the director and let the producer do it things. 

2.3 Design of some helper functions and datastructures

2.3.0 threadsafe_io:

I found the singleton mode is quite useful when working with I/O (basically just O in this program), so I use singleton mode in threadsafe_io mode and add and overwrite some functions to make it look like std::cout. I also define its single instance as safe_io in the utilities/const.hpp file, so that we can just include the file and get the instance.

2.3.1 threadsafe_menu:

A map structure consist of all of plays related information coming from dirctors. Its key is each director's id and its value is a pair of the dirctors status and its available plays. We support that more than one director can play a same name play, but we do not support a single director play two plays at the same time.

3.0 NOTE

I use [START], [STOP], [QUIT], as user commands, instead of start, stop, quit.
I test within a single machine, but it should be ok on different machine due to the reason that linuxlab seems doesn't allow me to expose its port.



-------------------|
3. Wrapper Facades |
-------------------|

In this lab, we use several wrapper facades.

Singleton mode: reactor instance/safe_io
Reactor mode: Reactor, all of our services are registered as reactor and reactively respond event.
Acceptor/Connector: producer and director.
Reader/Listener: reader service/ director/ ui_service


-----------------------------------------|
4. Insights, Observations, and Questions |
-----------------------------------------|

- In this lab, I find that the ACE frame help me to simplify the design of the program. But I also find that it is complicated. I have to take care of each step of my program, otherwise the frame will do something I didn't expect.

- It is a good idea to print a lot to find the bug.


=====================================================================================
==================================== INSTRUCTION ====================================
=====================================================================================

There is a script named test.sh, which will automatically clean files, compile the program and run the test. We save the output of the program into a file named tmp.txt we then compare it with the original script by the "diff" command.

    1: download the zip file into your CEC machine
    2: use the "cd" command getting into the directory of the zip file
    3: unzip the file via the command "unzip BingxinLiu-Lab3.zip"
    4: cd lab3/producer
    5: make producer program: make
    6: cd ../director
    7. make director program: make
    8. test the program

NOTE: this is a UI contained program, so I don't have a test script. Here is a work flow that I used to test my program
In the first window:
move into producer directory:
- cd /whatever/producer
run producer program (default port is 8086):
- ./main
then wait directors and test with your input

In the second windows or other windows
move into director dirctory:
- cd /whatever/director
run the director program
- ./main 8086 localhost 10 partial_hamlet_act_ii_script.txt
or
- ./main 8086 localhost 10 partial_hamlet_act_ii_script.txt partial_macbeth_act_i_script.txt
then test with your input in producer.

NOTE: the director directory has all of files needed to read for above two command.
====================================== END ==========================================