#!/bin/bash
cd qsub
bsub < qsub_clk.sh
#bsub < qsub_ff.sh
#bsub < qsub_omp.sh
#bsub < qsub_ser.sh
cd ..
