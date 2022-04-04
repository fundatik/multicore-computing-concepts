#!/bin/bash

nr_test=11

for (( k = 1; k < $nr_test; k++ )) ;
do
	echo rm test${k}/*.txt
	rm test${k}/*.txt
done

