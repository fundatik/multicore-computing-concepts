#!/bin/bash
times=$1
app=$2
c="1"
for (( k = 0; k < $times; k++ )) ;
do
       	out_file=${app}_$c.txt
      	echo Trial $k of running $app with SER and $c cores >&2
        export OMP_NUM_THREADS=$c;
        taskset -ac 0 bin/$app &> ${out_file}
        ts=$(grep "Time in seconds" ${out_file})
        echo $ts $c
       	#v=$(taskset -c 0-$((c - 1)) bin/$app.$class | grep -E 'Time in seconds' | grep -Eo '[0-9.]+')
        #echo $c , $v
done
rm ${out_file}
