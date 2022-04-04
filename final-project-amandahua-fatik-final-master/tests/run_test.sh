#!/bin/bash
# Arg1 is a comma separated list of benchmarks, Arg2 is a comma separated list of classes, Arg3 is the runtimes to build on
benchmark="mg"
machine="skylake"
test="test2"

classes=${1:-C}
runtimes=${2:-CLK,FF,OMP}
num_runs=${3:-1}
#output_path=${4:-"${HOME}/final-project-amandahua-fatik-final/tests/$machine/$test"}


if [[ $# -lt 1 ]]; then
	echo "Input a list of classes, options are S,W,A,B,C (default:${classes})"
	read -p "Classes: " classes_in
	classes=${classes_in:-$classes}
fi
if [[ $# -lt 2 ]]; then
	echo "Input list of runtimes, options are CLK,FF,OMP,SER(default: $runtimes)"
	read -p "Runtimes: " runtimes_in
	runtimes=${runtimes_in:-$runtimes}
fi
if [[ $# -lt 3 ]]; then
	echo "Input a number of times to run each trial (defaul: $num_runs)"
	read -p "Runtimes: " num_runs_in
	num_runs=${num_runs_in:-$num_runs}
fi
#if [[ $# -lt 4 ]]; then
#	echo "Input a path to output to (default: $output_path)"
#	read -p "Runtimes: " output_path_in
#	output_path=${output_path_in:-$output_path}
#fi

cd "$machine/$test"
for runtime in $(echo $runtimes | sed "s/,/ /g")
do
	for class in $(echo $classes | sed "s/,/ /g")
	do
		echo ${runtime}.${benchmark}.${class}
		now=$(date +"%T")
		echo "NPB-$runtime, current time : $now" >> ${benchmark}_${class}_${runtime}_${test}.txt
		./run-${runtime}.sh $num_runs ${benchmark}.${class} $machine $test >> ${benchmark}_${class}_${runtime}_${test}.txt
	done
done
