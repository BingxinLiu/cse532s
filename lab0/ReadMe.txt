CSE532S Fall 2021 Lab0

TEAM:
Bingxin Liu : bingxin.liu@wustl.edu

OVERVIEW:
In my program, there are three inner classes:
    Structed_line
    Play
    Thread_guard
Also, two function is decleared:
    read_thread
    argumens_sanity_check

For Structured_line class, it is a wrapper of a pair with an index
and a pair of one character's name and text, like the one described below:

[idex , ["character name", "character text"]]

For the Play class, it contains:
    a string of the play's name
    a structured lines map, an index as key, and a Structure line as value
    a mutex used to avoid race condition

The thread_guard class is the same as the one in [Williams] Chapter 2.

The "arguments_sanity_check" function is used to check if the arguments have
1. correct numbers, and 2. ".txt" suffix.
The "read_thread" function will take a reference to an instance of Play Object,
a const string for a character name, and a reference to an input stream, which refers
to the file that will be read. Then each thread will read a file and add a
structured line into structured lines map class member of the referred class object.

In the main function, the program will check the sanity of command-line
arguments, then it read the configuration file. For each line in the configuration file, 
it first reads the play name and stores it in a string. Then, it parses 
each line, gets the character name and his or her script file's name, and stores 
them into a map to avoid duplicate read since in c++, a map container doesn't 
allow duplicated insert.
After above, we initial a Play object with a play name.
Then, for each key-value pair in that map, we launch a thread to process the info 
via passed-in arguments, like the above described.
Finally, we print out the Play object by print method, which will read each key-value
pair in structured lines map and print out. Because the map container in c++ is an 
associated container, it has already been sorted by key, which is the index reflecting 
order in which the line is given in the play. Therefore, we can use an iterator to get 
sorted results from the map.


WRAPPER FACADES HELPED IN DESIGN:
In Play object, the program uses wrapper facades. It wrapper a mutex as a class member, 
and override the insertion operator so that whenever a thread try to insert something into 
the map, it is required to obtain the mutex first and then is allowed to insert a new one, 
which help us protect the map from race condition since every thread is trying to obtain 
the map when it is reading the file.
The program also uses RAII to wait for a thread to complete, which is similar to the one 
in [Williams] Chapter 2. As a result, even an exception happens a branched thread finally 
has to join the main thread.

INSIGHTS:
The wrapper facades and scoped Locking is really helpful for implement multi-threads 
program. They allow the program to automatically release the lock and join to the main 
thread and so we don't need to worry about the timing of those operations.

HOW TO RUN THE PROGRAM:
There is a script named test_script.sh, which will automatically clean files, compile 
the program and run the test. We save the output of the program into a file named tmp.txt 
we then compare it with the original script by the "diff" command.

1: download the zip file into your CEC machine
2: use the "cd" command getting into the directory of the zip file
3: unzip the file via the command "unzip BingxinLiu-Lab0.zip"
4: cd lab0/
5: chmod u=rwx,g=rx,o=r test_script.sh
6: run the script by "./test_script.sh"

The tail of the output should be something like:

1,3c1
< Hamlet Prince of Denmark ACT II Scene II A room in the Castle by William Shakespeare
<
< King.
---
> King.

We can see the output similar to that found in the hamlet_act_ii_scene_2.txt script fragment.
Otherwise, you can just run the program by "./read_play [arguments]".


EXTRA CREDIT
In extra credit part, we have a program named extra_credit.cpp. The program will read each line 
in the hamlet_act_ii_scene_2.txt, put them into each character's text file, and then generate 
a configuration file.
There is a script in the file, which will run the program and compare each generated file with 
original file. If everthing is ok, there should not be any output relating to the command, except: 

1c1
< Hamlet Prince of Denmark ACT II Scene II by William Shakespeare
---
> Hamlet Prince of Denmark ACT II Scene II A room in the Castle by William Shakespeare

This is because the different of play name.

HOW TO USE:

1: download the zip file into your CEC machine
2: use the "cd" command getting into the directory of the zip file
3: unzip the file via the command "unzip BingxinLiu-Lab0-ExtraCredit.zip"
4: cd lab0_extra_credit/
5: chmod u=rwx,g=rx,o=r test_script.sh
6: run the script by "./test_script.sh"
