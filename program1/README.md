Author: Joshua Kluthe
Date: 2017.04.17

This program was written for CS 475 Parallel Programming course at 
Oregon State University, Sprint '17.

The program uses OpenMP multiprocessing to calculate the volume between 
two Bezier surfaces. Megacalcs per second are used to gauge the 
performance on different numbers of threads and nodes, where a calc is 
a single for loop iteration that calculates a fraction of the total 
volume. The data prints to a file named 'data.csv'.

To run, simply use the command 'python3 run_prog1.py'.

