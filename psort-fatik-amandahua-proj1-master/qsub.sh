#!/bin/sh

#BSUB -q linuxlab-cse539
#BSUB -o sort.out.%J
#BSUB -e sort.out.%J
#BSUB -J sort

taskset -c 0 ./sort 10000000 0 1 1 
taskset -c 0 ./sort 10000000 1 1 1 
taskset -c 0-1 ./sort 10000000 2 1 1 
taskset -c 0-3 ./sort 10000000 4 1 1 
taskset -c 0-7 ./sort 10000000 8 1 1 
taskset -c 0-11 ./sort 10000000 12 1 1 
taskset -c 0-15 ./sort 10000000 16 1 1 


 
