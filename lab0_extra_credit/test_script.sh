#!/bin/sh
# This is a test for cse532s lab0

$string_in="in"

# clear old things
rm *.txt
rm extra_credit

# get needed text file
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/hamlet_act_ii_scene_2.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/hamlet_ii_2_config.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/Guildenstern.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/King.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/Queen.txt --no-check-certificate
wget https://www.cse.wustl.edu/~cdgill/courses/cse532/lab0/Rosencrantz.txt --no-check-certificate

# add original file with a copy suffix to be compared later
mv hamlet_ii_2_config.txt hamlet_ii_2_config_copy.txt
mv Guildenstern.txt Guildenstern_copy.txt
mv King.txt King_copy.txt
mv Queen.txt Queen_copy.txt
mv Rosencrantz.txt Rosencrantz_copy.txt


# compile program
g++ -Wall -std=c++11 -pthread ./extra_credit.cpp -o extra_credit

# execute the program
./extra_credit hamlet_act_ii_scene_2.txt hamlet_ii_2_config.txt Hamlet Prince of Denmark ACT II Scene II by William Shakespeare

# compare with original script
diff hamlet_ii_2_config.txt hamlet_ii_2_config_copy.txt
diff Guildenstern.txt Guildenstern_copy.txt
diff King.txt King_copy.txt
diff Queen.txt Queen_copy.txt
diff Rosencrantz.txt Rosencrantz_copy.txt

