#!/bin/bash
# This is a test script for cse532s lab0

# clear old things
rm *.txt
rm lab1

# compile program
g++ -Wall -std=c++11 -pthread ./src/play.cpp ./src/player.cpp ./src/utilities.cpp ./src/main.cpp -o lab1

# NORMAL TEST
    echo "NORMAL TEST:"

    # creat a file to save the output
    touch tmp.txt

    # excuet the program
    ./lab1 ./scripts_and_config/hamlet_ii_2_config.txt >> tmp.txt

    # compare with original script
    diff -w ./scripts_and_config/hamlet_act_ii_scene_2.txt ./tmp.txt

    # remove tmp.txt
    rm tmp.txt

# BADLY FORMED CONTENT AND MISORDERED LINES TEST
    echo "BADLY FORMED CONTENT MISORDERED LINES TEST:"

    # creat a file to save the output
    touch tmp.txt

    # excuet the program
    ./lab1 ./scripts_and_config/hamlet_ii_2_config_badly_formed_content.txt >> tmp.txt

    # compare with original script
    diff -w ./scripts_and_config/hamlet_act_ii_scene_2_badly_formed_content.txt ./tmp.txt

    # remove tmp.txt
    rm tmp.txt