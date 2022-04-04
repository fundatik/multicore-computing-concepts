#!/bin/bash

PROCESSORS="1 2 4 8 16"

for P in $PROCESSORS; do
  echo "P=$P" | tee -a fib-timing.txt
  CILK_NWORKERS=$P ./fib 42 | tee -a fib-timing.txt

  echo "P=$P" | tee -a cholesky-timing.txt
  CILK_NWORKERS=$P ./cholesky -n 4000 -z 40000 | tee -a cholesky-timing.txt

  echo "P=$P" | tee -a fft-timing.txt
  CILK_NWORKERS=$P ./fft -n 67108864 | tee -a fft-timing.txt

  echo "P=$P" | tee -a heat-timing.txt
  CILK_NWORKERS=$P ./heat -nx 2048 -ny 2048 -nt 500 | tee -a heat-timing.txt

  echo "P=$P" | tee -a lu-timing.txt
  CILK_NWORKERS=$P ./lu -n 4096 | tee -a lu-timing.txt

  echo "P=$P" | tee -a mm_dac-timing.txt
  CILK_NWORKERS=$P ./mm_dac -n 2048 | tee -a mm_dac-timing.txt

  echo "P=$P" | tee -a nqueens-timing.txt
  CILK_NWORKERS=$P ./nqueens 14 | tee -a nqueens-timing.txt

  echo "P=$P" | tee -a strassen-timing.txt
  CILK_NWORKERS=$P ./strassen -n 4096 | tee -a strassen-timing.txt
done
