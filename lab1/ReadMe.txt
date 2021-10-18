=====================================================================================
=========================== CSE532S Fall 2021 LAB1 REPORT ===========================
=====================================================================================

---------------|
1. Team Member |
---------------|
    Bingxin Liu : bingxin.liu@wustl.edu


----------------------|
2. Overview of Design |
----------------------|

Structure of the dirctory:
    lab1
    ├── ReadMe.txt
    ├── scripts_and_config
    │   ├── Cse532s_badly_formed_content.txt
    │   ├── Guildenstern.txt
    │   ├── Guildenstern_badly_formed_content.txt
    │   ├── King.txt
    │   ├── King_badly_formed_content.txt
    │   ├── Queen.txt
    │   ├── Queen_badly_formed_content.txt
    │   ├── Rosencrantz.txt
    │   ├── Rosencrantz_badly_formed_content.txt
    │   ├── hamlet_act_ii_scene_2.txt
    │   ├── hamlet_act_ii_scene_2_badly_formed_content.txt
    │   ├── hamlet_ii_2_config.txt
    │   └── hamlet_ii_2_config_badly_formed_content.txt
    ├── src
    │   ├── main.cpp
    │   ├── play.cpp
    │   ├── play.hpp
    │   ├── player.cpp
    │   ├── player.hpp
    │   ├── utilities.cpp
    │   └── utilities.hpp
    └── test.sh

- scripts_and_config:   used to store various txt files, including configure file, script file, and per-character script file. Also, some test file is included in this director
- src:                  used to store source code
- test.sh:              a command line script to compile and run test automatically.

2.1 Design of the Structured_line class
The Structured_line class is defined in the file "play.hpp", which define a data structure working the same as a c++ pair. Each of its instance will have a string storing the script line belongs to which character, and a string storing the script line itself.

2.2 Design of the Play class
The Play class is defined in the file "play.hpp".
It has six members:
    play_name                   : the name of the play which the Play class instance on be half of,
    counter                     : a counter recording which line the play is reciting,
    recite_mutex                : working as a lock to keep the order of output,
    recite_condv                : a condition variable used to wait and wake up a player instance to recite its script,
    current_character           : indicate the current reciting character,
    has_recite_first_character  : used as a flag to indicate if we has recited the first character, so that if it is the turn for another charcter, we print a new line.
It has a function member except constuctor:
    recite():
    The recite function receives an iterator pointing to a structured line, which contains all of the information we need to recite the script. In the inner of the function, we first obtain the mutex to keep us from race condition, then we check if the order of the structured line the iterator pointing to has a larger number than the counter. If it is not, it means that it is not the turn of this structure line to recite so that we release the mutex and wait until next time someone called notify_all(). Otherwise, this structured line may turn to recite, so we test the situation. If the order number is larger than the current counter, we know that the order may be disturbed, so we print out an error message into the standard error stream. Then we increment the iterator with one to try to continue with the next one. If the order is equal to the counter, then we know that this structured line is exactly what we want. We dereference it and print out its contents with some format if necessary. Finally, we increment both the counter and iterator, release the lock and notify all of the waiting thread to check if it is their turn.

2.3 Design of the Player class
The Player class acts as a wrapper of a new thread. Whenever we construct a new instance of Player class, we are actually spawning a new thread and making it do some specific function. The class uses Interface Wrapper Facades and RAII, which will be described in section 3.
It has five members:
    lines   : an ordered map, the order of line as its key, a structured line as its value. The map will automatically ordered by its key, so we don't need to bother about sort all of lines. This member stores all of the strcipt lines of a specific character,
    character   : with string type, store the name of the character,
    mythread    : store the inner thread it has. we use it to control each player thread,
    input_file_stream   : an input file stream, we use it to extra each line from the character's script file and store each one as a structured line into lines.
    play    : a reference to the play instance, so that we can call the Play class's recite function.
It has four function members:
    read()  : read each line of the input file, extract as a structured line, then insert them into our map
    act()   : from the beginning of one character's first script line to the very end, call the blocking member function recitre of the Play class, which will wait if it is not in this character's turn and recite if it is.
    enter() : an enter point of player's worker thread. After the thread is initialized, it calls read() and act() function to act as playing the player's role.
    exit()  : after the player finish it role, check if the thread is joinable. If it is then join with the main thread.
So, in this case, we use enter() and exit() member functions to control when and where to run and stop the player thread. We use the read() and act() member functions to control the behaviors of the player thread.

2.4 Design of the main function
In the main function, we first do the sanity check of the arguments. Then we start to read the name of the play by reading the config file and skipping each empty line so that we are able to construct an instance of Play object. After that, we continue to read and parse the following lines of the config file. Getting enough info, including the character's name and the character's script file, after establishing an input file stream, we construct an instance of player for each character and push it into a vector. Finally, for each item in that vector, we call the enter() and exit() to let the thread be released and stopped automatically.


-------------------|
3. Wrapper Facades |
-------------------|

In this lab, we use Interface Wrapper Facade and RAII.
A player Interface including enter, exit, read, and act behaviors, which defines the behaviors of the player threads.
We also implement RAII by test and join the thread in the Player object's destructor so that we don't have to keep an eye on joining if the user of the class forgets to invoke the exit() function to stop the thread.


-----------------------------------------|
4. Insights, Observations, and Questions |
-----------------------------------------|

- In this lab, I find that implementing the interface wrapper facade makes the class user easy to use and understand our class. The behaviors of the class are abstracted so that the user doesn't have to be bothered with the detail of the implementation of a class. Instead, they only need to find out the API and use them on demand.
- I also noticed that different design patterns could be combined to make the code beautiful. For example, in this case, we use RAII and Interface at the same time.
- It is a good idea to focus on behaviors instead of data. Unlike what we have done in this lab, each player instance has the same behaviors but its own data. It is automatically splitting data into separate threads (via input file). Therefore, instead of contenting on a big area of shared data, we manage to let each thread manages their own data and obtains shared data as least as possible.
- Actually, our code still has a problem. That is, if a player thread throws an exception, the main thread can not catch it, which may cause the player thread to fail to join. We may use promise and future to figure out that problem.


=====================================================================================
==================================== INSTRUCTION ====================================
=====================================================================================

There is a script named test.sh, which will automatically clean files, compile the program and run the test. We save the output of the program into a file named tmp.txt we then compare it with the original script by the "diff" command.

    1: download the zip file into your CEC machine
    2: use the "cd" command getting into the directory of the zip file
    3: unzip the file via the command "unzip BingxinLiu-Lab1.zip"
    4: cd lab1/
    5: chmod u=rwx,g=rx,o=r test.sh
    6: run the script by "./test.sh"


=====================================================================================
==================================== EXTRA CREDIT ===================================
=====================================================================================

The implement of extra credit is pretty straightforward. We first check if the SCRAMBLE is set. If it is set, we will shift each index of the argument with 1 to reuse the code of lab0's extra code. Then, for each line of the script file, we didn't append them into their own files. Instead, we push it into a vector. Then, we set the seed of the random function and use the random_shuffle function, which is provided in the algorithm head file. After that, we append each item in the vector into its own file.

INSTRUCTION OF EXTRA CREDIT
    1: download the zip file into your CEC machine
    2: use the "cd" command getting into the directory of the zip file
    3. use the "mv" command to move the zip file under the same dirctory where the directory of lab1 is located
    4: unzip the file via the command "unzip BingxinLiu-Lab1-ExtraCredit.zip"
    5: cd lab1_extra_credit/
    6: chmod u=rwx,g=rx,o=r test.sh
    7: run the script by "./test.sh"

NOTE :
In this case, we will make use of the main program of lab0 to test the extra credit program, so we have to put them under the same directory to make the test script runnable.


=====================================================================================
==================================== EVALUATION =====================================
=====================================================================================

FOR MAIN PART:
We use the test script to test the code under normal and abnormal situations.
First, we compile the program and run the code with the default setup (txt files are located in scripts_and_config file using the unchanged files on the website). We redirect the output into a temporary file name tmp.txt so that we can use it to compare with the original file hamlet_act_ii_scene_2.txt. The commands are shown below:

./lab1 ./scripts_and_config/hamlet_ii_2_config.txt >> tmp.txt

diff -w ./scripts_and_config/hamlet_act_ii_scene_2.txt ./tmp.txt

We also test the program in the situation that the file is not well formated.
We changed several files to test different situations all togather:
1. we add a new character named Cse532s who don't have any script(even silent line), so we should skip this character. To make this we "touched" a new file named Cse532s_badly_formed_content.txt and add it into second configure file named hamlet_ii_2_config_badly_formed_content.txt. Note, below description will assume we have a "_badly_formed_content" suffix.
2. In Guildenstern's script we delete the content of the line with order number 34 but keep the order number.
3. In King's script we shuffle the order of the lines.
4. In Queen's script we add a line filled with several whitespaces.
5. In Rosencrantz's script we add several empty line.
As all of the side effects we would expact, we change the original script and save it into the new file named hamlet_act_ii_scene_2_badly_formed_content.txt

Therefore, the tail of the final ouput from running test script should be something like:
    [bingxin.liu@linuxlab006 lab1]$ ./test.sh
    NORMAL TEST:
    BADLY FORMED CONTENT MISORDERED LINES TEST:
    [bingxin.liu@linuxlab006 lab1]$
    As the routine of Linux, nothing means everthing is ok.

FOR EXTRA CREDIT PART:
It is pretty like what we do in the normal situation of the main part, except that we use the extra credit part to shuffle and generate each character's script from the original script. The tree structure of the directory is shown below:

    .lab1_extra_credit
    ├── extra_credit.cpp
    ├── hamlet_act_ii_scene_2.txt
    └── test.sh

We still compile a runnable program from the main part, and we compile the extra_credit program. Then, we first run the extra_credit program to generate configure file and script files. Finally, we run the main part program with generated files and compare the result with the original script file hamlet_act_ii_scene_2.txt.
Thus the tail of the final output from running the test script should be something like:

    2021-10-13 23:04:05 (132 MB/s) - ‘hamlet_act_ii_scene_2.txt’ saved [1774/1774]

    NORMAL TEST:
    [bingxin.liu@linuxlab006 lab1_extra_credit]$

====================================== END ==========================================