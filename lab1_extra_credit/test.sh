#!/bin/sh
# This is a test for cse532s lab0

# clear old things
rm *.txt
rm lab1_extra_credit
rm lab1

# get script
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab1/hamlet_act_ii_scene_2.txt --no-check-certificate

# compile lab1 extra credit program
g++ -Wall -std=c++11 -pthread ./extra_credit.cpp -o lab1_extra_credit
# compile lab1 program
g++ -Wall -std=c++11 -pthread ../lab1/src/play.cpp ../lab1/src/player.cpp ../lab1/src/utilities.cpp ../lab1/src/main.cpp -o lab1

# NORMAL TEST
echo "NORMAL TEST:"

# execute the program
./lab1_extra_credit SCRAMBLE hamlet_act_ii_scene_2.txt hamlet_ii_2_config_scrambled.txt Hamlet Prince of Denmark ACT II Scene II by William Shakespeare
./lab1 hamlet_ii_2_config_scrambled.txt >> tmp.txt

# compare with original script
diff -w hamlet_act_ii_scene_2.txt tmp.txt

