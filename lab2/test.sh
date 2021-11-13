#!/bin/bash
# This is a test script for cse532s lab0

# clear old things
rm tmp1.txt tmp.txt

# compile program
make

# RUN TEST
    echo "RUN TEST:"

    # creat a file to save the target output
    cat ./script_files/hamlet_act_ii_scene_* > tmp.txt

    # excuet the program
    ./lab2 ./config_files/partial_hamlet_act_ii_script.txt > tmp1.txt

    # compare with original script
    diff -bB ./tmp1.txt ./tmp.txt
