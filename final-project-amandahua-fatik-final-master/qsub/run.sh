#!/bin/bash
benchmark="mg"
class="C"
num_runs="1"

#cd NPB-$runtime

cd ../NPB-$1
echo "$benchmark, $class, $runtime"
echo "./NPB-$runtime/run.sh $num_runs ${benchmark}.${class}"
./run.sh $num_runs ${benchmark}.${class}
cd ..
cd ..
