#!/bin/bash
times="1"
app="mg.C"
machine="skylake"
test="test1"


#./run-CLK.sh $times $app $machine $test
./run-OMP.sh $times $app $machine $test
#./run-FF.sh $times $app $machine $test

