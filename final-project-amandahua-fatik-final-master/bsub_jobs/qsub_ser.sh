#!/bin/sh

#BSUB -q linuxlab-cse539
#BSUB -o mg.ser.%J
#BSUB -e mg.ser.%J
#BSUB -J run.sh

echo "Start"
./run.sh SER
echo "End"
