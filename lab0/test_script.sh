#!/bin/sh
# This is a test for cse532s lab0

# clear old things
rm *.txt
rm read_play

# get needed text file
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/hamlet_act_ii_scene_2.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/hamlet_ii_2_config.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/Guildenstern.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/King.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/Queen.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/Rosencrantz.txt --no-check-certificate

# compile program
g++ -Wall -std=c++11 -pthread ./read_play.cpp -o read_play

# creat a file to save the output
touch tmp.txt

# execute the program
./read_play hamlet_ii_2_config.txt >> tmp.txt

# compare with original script
diff ./tmp.txt ./hamlet_act_ii_scene_2.txt
