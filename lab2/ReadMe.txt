=====================================================================================
=========================== CSE532S Fall 2021 LAB2 REPORT ===========================
=====================================================================================

---------------|
1. Team Member |
---------------|
    Bingxin Liu : bingxin.liu@wustl.edu


----------------------|
2. Overview of Design |
----------------------|

Structure of the dirctory:
lab2
├── Makefile
├── config_files
│   ├── hamlet_ii_1a_config.txt
│   ├── hamlet_ii_1b_config.txt
│   ├── hamlet_ii_2a_config.txt
│   └── partial_hamlet_act_ii_script.txt
├── script_files
│   ├── Guildenstern_hamlet_ii_2a.txt
│   ├── King_hamlet_ii_2a.txt
│   ├── Ophelia_hamlet_ii_1b.txt
│   ├── Polonius_hamlet_ii_1a.txt
│   ├── Polonius_hamlet_ii_1b.txt
│   ├── Queen_hamlet_ii_2a.txt
│   ├── Reynaldo_hamlet_ii_1a.txt
│   ├── Rosencrantz_hamlet_ii_2a.txt
│   ├── hamlet_act_ii_scene_1a.txt
│   ├── hamlet_act_ii_scene_1b.txt
│   ├── hamlet_act_ii_scene_2a.txt
│   ├── hamlet_ii_1a_config.txt
│   └── hamlet_ii_1b_config.txt
└── src
    ├── ReadMe.txt
    ├── def.hpp
    ├── director.cpp
    ├── director.hpp
    ├── lab2.cpp
    ├── play.cpp
    ├── play.hpp
    ├── player.cpp
    ├── player.hpp
    ├── utilities.cpp
    └── utilities.hpp

- config_files:   
    Used to store configuration files, including scene configure files and each fragment's configur files.
- script_files:
    Used to store each character's script files and the whole play script files.
- src:
    Used to store source code
- test.sh:
    a command line script to compile and run test automatically.

2.0 Logic Overview
In thi lab, there are 3 main roles: director, play, and player. The program will first construct a director instance, which parses the configure file and according to the actual situtation decides how many players the director will recruit. At the same time an instance of play object will be constructed. The players will enter to, act on, and exit from the play. If the only on-the-stage player exit, it is time to change to another scene. The dirctor will recruite enough number of players to perform in this scene. Then players will compete for the leader position, and the first players trying to become a leader will actually become as a leader. After that, the leader will on behalf of other players asking director to assign each players a character including the leader itself. Assigned character, all of players read their script and rescite their lines in order. One scene after one scene, until the last one.

2.1 Data Structure
2.1.1 Structured_line class
The Structured_line class is defined in the file "play.hpp", which define a data structure working the same as a c++ pair. Each of its instance will have a string storing the script line to which the character belongs, and a string storing the script line itself.
Each player

2.1.2 Design of the Director class
Constructing a director, the program will first parse the configure file for the whole play and get each configure file for a single scene fragment. Then, for each scene fragment's file it will parse the info and get character and character's script file name. At the same time, the director will keep tracking the maximum number of characters appears in either two consecutive scene fragment, compare it with the minimum number of players specified by the argument and finally select the larger one of them. Then the director will recruit players and let them start working.
Then the dirctor will start the play.
First, the dirctor wake up players by setting the needed_player_num, each resumed player will decrement this variable by one, until it is changed to zero, which means the director has enough players.
Then the dirctor will set a number of not ready players number, which at first is equal to the number of players in this scene, so that if a player is ready, it will decrement this value by one. Until the value is decremented to zero, all players are ready to play.
At the same time the director will wake up a player waiting on the condition variable. It will become the leader, who will in turn try to wake up all players to become a activated player, only an activated player can be assigned with a character. Then the leader will let the director assign character, so that the dirctor can also cue a specific number of players according to the requirement of a scene fragment, change their status from idle to working, and assign each player its own character. The number of cued player should be exactly the same as the number of activated player.
Finally, the director release its control to the play and let each player plays in turn. Each time changing a fragment of scene to another, the leader of players will ask director to assign/cue characters according to the index of the scene.

2.1.3 Design of the Player class
The Player class acts as a wrapper of a new thread. In the thread, each player will do a busy loop until the play has finished. The loop described as below:
1. First the player will check whether it it the time to stop it work, which means the play is finished. The player does it via check a flag in the play object.

2. After that the player will wait on a condition variable until the dirctor need some player to play, which means needed player number variable is greater than zero or the play is finished. If the scene indeed has finished, it just go the next step. Otherwise, the player wake up itself and set itself as activated.

3. The player check if itself activated. If not, in this case, it means the player wake up because the play is done. Then the player go back to step one. Otherwise, the player wake up because it has work to be finished. First, it check if itself can be a leader by check has_player flag. If it can, then becomes a leader and try to wake up all of the players to compete a position as follower and, at the same time, it wait until enough players wake up. Then it ask director to assign characters according to current flagment index. After that, the leader change itself as a normal follower to enter to the current fragment of the scene:
    3.1 it first wait a character.
    3.2 Then it read its script lines.
    3.3 After that, it enter to the play and wait for every other player finish reading script(not ready)
    3.4 Then it wait for its turn to recite a line, until all of its script has been recite.
    3.5 After the act, it exit from the play and clear itself as an inactive player and return to the first step.

2.1.4 Design of the play function
The play object plays like a scheduler, everytime a player enter to the play, it check if it is the player's turn to enter. After the player recite a line it increment its line counter to keep tracking the progress of the play. If the last player in current scene finish its act and exit, the play will increment its fragment counter and reset the line counter to one. Then it set the number of player needed by next scene and the number of not ready player, clear has_leader flag, and finally wake up one player to prepare the next scene. If it is the last scene, it will set the finished flag and wake up all the players so that they can check the flag and return from the busy loop.

2.1.5 Design of the main function
Right now, the main function becomes extremely simple, it just construct a director object and wait for the play finished.


-------------------|
3. Wrapper Facades |
-------------------|

In this lab, we use Interface Wrapper Facade and RAII.
A player Interface including enter, exit, read, and act behaviors, which defines the behaviors of the player threads.
We also implement RAII by test and join the thread in the Player object's destructor so that we don't have to keep an eye on joining if the user of the class forgets to invoke the exit() function to stop the thread.
Most importantly, we use leader and follower pattern, which is efficient since there isn't any operation of new and destruct thread, which cause huge overhead.


-----------------------------------------|
4. Insights, Observations, and Questions |
-----------------------------------------|

- In this lab, I find that implementing the multithread program become harder and harder, we need to take care ever edge case.
- I also noticed that we can not presume any order about which thread run first and which one is the follower. If you have to, use confition variable.
- It is a good idea to assert everything you need in time. Sometimes, a fatal fault happens but it only appears in the future, so you may not aware the point easily.


=====================================================================================
==================================== INSTRUCTION ====================================
=====================================================================================

There is a script named test.sh, which will automatically clean files, compile the program and run the test. We save the output of the program into a file named tmp.txt we then compare it with the original script by the "diff" command.

    1: download the zip file into your CEC machine
    2: use the "cd" command getting into the directory of the zip file
    3: unzip the file via the command "unzip BingxinLiu-Lab2.zip"
    4: cd lab2/
    5: chmod u=rwx,g=rx,o=r test.sh
    6: run the script by "./test.sh"


=====================================================================================
==================================== EXTRA CREDIT ===================================
=====================================================================================

We have implement the part of extra credit about override flag. The program could run with correct argument of number of the player, which should be greater than the maximum number of needed players over all of the scene. This is because our player can not act more than one roles. The play need all of needed players ready to play, otherwise it will blocked forever.

We don't have a test script because we can not guarantee than each run could be finished.

INSTRUCTION OF EXTRA CREDIT
    1: download the zip file into your CEC machine
    2: use the "cd" command getting into the directory of the zip file
    3. use the "mv" command to move the zip file under the same dirctory where the directory of lab1 is located
    4: unzip the file via the command "unzip BingxinLiu-Lab2-ExtraCredit.zip"
    5: cd lab2_extra_credit/
    6: make
    7: ./lab2 [with arugments you want]

====================================== END ==========================================