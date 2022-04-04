#!/bin/bash
#Arg1 is a comma separated list of classes, Arg2 is the runtimes to build on
rm NPB-CLK/bin/* >/dev/null 2>&1
rm NPB-FF/bin/* >/dev/null 2>&1
rm NPB-OMP/bin/* >/dev/null 2>&1
rm NPB-SER/bin/* >/dev/null 2>&1

classes=${1:-C}
runtimes=${2:-CLK,FF,OMP,SER}
if [[ $# -lt 1 ]]; then
	echo "Input a list of classes, options are S,W,A,B,C (default:$classes)"
	read -p "Classes: " classes_in
	classes=${classes_in:-$classes}
fi

if [[ $# -lt 2 ]]; then
	echo "Input a list of runtimes, options are CLK,FF,OMP,SER (default: $runtimes)"
	read -p "Runtimes: " runtimes_in
	runtimes=${runtimes_in:-$runtimes}
fi
for runtime in $(echo $runtimes | sed "s/,/ /g")
do
    cd NPB-$runtime;
    #make cleanall;
	for class in $(echo $classes | sed "s/,/ /g")
	do
		make mg CLASS=$class
	done
	cd ..;
done
