#!/bin/sh

#BSUB -q linuxlab-cse539
#BSUB -o mg.clk.%J
#BSUB -e mg.clk.%J
#BSUB -J run.sh

echo "Start"
./run.sh CLK
echo "End"
