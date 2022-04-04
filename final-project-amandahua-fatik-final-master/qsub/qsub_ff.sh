#!/bin/sh

#BSUB -q linuxlab-cse539
#BSUB -o mg.ff.%J
#BSUB -e mg.ff.%J
#BSUB -J run.sh

echo "Start"
./run.sh FF
echo "End"
