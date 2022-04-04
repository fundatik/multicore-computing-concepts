#!/bin/sh

#BSUB -q linuxlab-cse539
#BSUB -o mg.omp.%J
#BSUB -e mg.omp.%J
#BSUB -J run.sh

echo "Start"
./run.sh OMP
echo "End"
