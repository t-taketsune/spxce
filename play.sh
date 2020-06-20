#!/bin/bash

rm spxce

gcc -ansi -Wall -o spxce *.c -lncurses

./run_game.sh spxce

