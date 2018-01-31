#!/bin/bash

for file in ` find ./graphs1 -name "*.alist" | sort`
do
    `cat $file > temp`
    ../ChromaticNum/vertex_coloring_solver/lp_chromatic $file >> temp
    ./qubo_solver_cplex/qubo ${file%.*}.H >> temp
    ./verification < temp
done
rm -rf temp
