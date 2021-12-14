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

In the main function of the dirctor program, we will fist parse its command line arguments like what we did in lab2. Then we get an instance of director object and register it as a ACE Event handler. Finally we launch the event loop and waiting until the loop stopped.

The director will first parse those config files like what we did in lab2, except that the director will also establish a connection with the Producer via ACE_SOCK_Connector. After the connection established, the director will send '[director_id] 0' to the producer. Then the producer will return a unique id for this director as its id. Then the director reactively listen from the producer, if the producer send star, stop, quit command, it will reponse respectively.

    1. If the producer send back the id, the director instance will set its director_id member with it, so that the director get its own unique id.

    2. If the director get the start command it will start the play, which means it will set the play name and its need configuration files, and recruit enough players, and finally start the play like what we did in the lab2.

    3. If the director get the stop command, it will set the play object's finished flag and notify all of the waiting/playing players, so that the players will notice that change and exit from the stage and/ or exit from the play.

    4. The responding for quit command is pretty the same as the stop command. The director will set the finished flag and notify players. Then, it will send a confirm message back to the producer, so that producer can release itself when every director quited. After that, the director will stop the event loop and release the sources it hold.

Note, the director will also set the finshed flag, notify players, send a quit confirm message to producer, and finially release itself when the user hit Ctrl-C, as what it will do when it receive quit command.

2.2 Design of the Producer program

The main function of the producer program will also parse the command line arguments at first. Then it will launch a ACE_SOCK_Acceptor to listenning on the assigned port. After that, a producer class instance will be established and accept the existed connector as its own connecter. Now, we can register the producer instance as an ACE Event handler for connections and signal (Ctrl-C), and we can launch the reactor event loop to reponse directors and user's signal reactively.

The producer program provides three service class which are inherited from ACE_Event_Handler.

The first one is UI service, it will listen on standard input, parse the command, and let the producer send messages to corresponding socket, which will end up with reaching one director. Also, the ui_service will provide a new menu each time the play started/stopped/finished or new director connected with its new supported plays. The instance of producer object will first establish a UI serice instance and register it into the reactor event loop. For those commands comming from user interface:

    1. if user shoot a start command, the ui service will first get the play via the input index, then it will find an available director from its menu, which is eastablished and updated during communicating with directors. If ever find an available one, it will update the menu for user and send message for that director to let it play the play.

    2. if user want quit a play, the ui service will check its menu and find a director that is playing the play, and send a message to it to let the director stop.

    3. if the user want to leave, they will send quit command. Then the ui service will first remove it self from the event loop. This is because we don't want to listenning on user's input anymore. We can not just stop the event loop because we want reader service, which is used to listenning on directors' message, continue its services, so that we can know when we can release all of the resources. Then, the ui service will let the producer send a quit message to all of directors which have registered itself in the producer. After that, we launch a new thread to wait on the condition that all of the directors have left. Then we stop the event loop and release all of the resources.

The second one is producer(class) service, which is similar as an acceptor in Connector/Acceptor mode. It accept a connection request, establish the connection and hand over the connection to a new established reader service, which will be used as a receiver for director's message.

The third one is reader service, it is built by the producer whenever it receive and establish a connection with the director. After that, the reader will on behalf of the producer to do all the communication with this specific director. It will send messages to director and recieve and parse the message coming from the director and let the producer do it things. For those messages comming from directors:

    1. if the director send a message of "[director_id] 0", the producer will send back the id the producer assigned to it, so that the director as the director's unique id, which is the same as the reader_service's id. After that, the UI service and the producer know the map relation between id and director.

    2. if the director send a message to register one of its supported plays in the form of "[PLAY] ***". The reader will let the producer register the play with its id and play name. Then the producer will refresh the menu to the user.

    3. if the producer received a started confirm message, it will change the status of that director from UNCLEAR to UNAVAILABLE, which is the side effect of a user shooting start command.

    4. if the producer received a stopped confirm message, it will set the status of that director as AVAILABLE and refresh the menu. Note, only available director will appear on the menu.

    5. if receiving a quit comfirm message, the reader service will first remove itself from the reactor loop. Then it will remove the director record corresponding to itself from the menu and erase itself from the map mapping the director id and its socket. After that, the producer will be no longer aware of the exist of this service handler, so that it can remove itself and release resouces.

2.3 Design of some helper functions and datastructures

2.3.0 threadsafe_io:

I found the singleton mode is quite useful when working with I/O (basically just O in this program), so I use singleton mode in threadsafe_io mode and add and overwrite some functions to make it look like std::cout. I also define its single instance as safe_io in the utilities/const.hpp file, so that we can just include the file and get the instance.

2.3.1 threadsafe_menu:

A map structure consist of all of plays related information coming from dirctors. Its key is each director's id and its value is a pair of the dirctors status and its available plays. We support that more than one director can play a same name play, but we do not support a single director play two plays at the same time.

If it is the time to refresh the menu, the menu will collect all of plays with its available directors according to the record in its datastructures. A director with UNCLEAR or UNAVAILABLE status will not be regared as playable.

3.0 NOTE

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

- ACE is a wonderful frame but we should take much care about resources releasing and the time stop and close the event handler.


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

HERE IS AN ORDER HOW I TEST THE UI:
1. start all plays with start command
2. in a director program hit Ctrl-C to see if the program exit correctly, and in the producer program it should never show up.
3. in the producer program hit stop and then start teh same play to see if everything is ok.
4. Then hit quit or Ctrl-C to test if the producer and directors can be stopped correctly.

NOTE: the director directory has all of files needed to read for above two command.
====================================== END ==========================================
