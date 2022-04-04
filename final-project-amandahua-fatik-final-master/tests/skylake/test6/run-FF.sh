#!/bin/bash
times=$1
app=$2
machine=$3
test=$4
imp="FF"


cd ../../../NPB-FF/

# second second, scatter
declare -a lst_nr_cores=("14" "16" "18" "20" "22" "24")
declare -a lst_core_ids=("0,2,4,6,8,10,12,1,3,5,7,9,11,13" "0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15" "0,2,4,6,8,10,12,14,16,1,3,5,7,9,11,13,15,17" "0,2,4,6,8,10,12,14,16,18,1,3,5,7,9,11,13,15,17,19" "0,2,4,6,8,10,12,14,16,18,20,1,3,5,7,9,11,13,15,17,19,21" "0,2,4,6,8,10,12,14,16,18,20,22,1,3,5,7,9,11,13,15,17,19,21,23")

for (( k = 0; k < $times; k++ )) ;
do
    for i in ${!lst_nr_cores[@]}; do
	c=${lst_nr_cores[$i]}
       	out_file=${app}_${c}_${imp}.txt
      	echo Trial $k of running $app with $imp and $c cores >&2
	export FF_NUM_THREADS=$c; export OMP_NUM_THREADS=$c; export OMP_PROC_BIND=true;
	likwid-pin -C ${lst_core_ids[$i]} bin/$app &> ../tests/$machine/$test/${out_file}
	ts=$(grep "Time in seconds" ../tests/$machine/$test/${out_file})
        echo $c, $ts
    done
done
cd ../tests/$machine/$test
