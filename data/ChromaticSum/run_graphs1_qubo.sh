#!/bin/bash
function chromaticSumQUBO(){
    for file in ` find $1 -name "*.alist"`
    do
        `$2 $file`
    done
}

function runQuboSolver(){
    i=1
    for file in ` find $1 -name "*.H" | sort`
    do
        `echo "[$i]">> $2`
        `echo ${file%.*} >> $2`
        #echo $3 $file
        `$3 $file >> $2`
        `echo -e "\n\n">>$2`
        let i=i+1
    done
}

QUBOSOLVER="./qubo_solver_cplex/qubo"
CHRO_SUM_QUBO="./chromatic_sum_qubo"
GRAPH_PATH="./graphs1"
OUTPUT="out_graphs1_qubo.out"
rm -f $GRAPH_PATH/*.H
rm -f $OUTPUT
chromaticSumQUBO $GRAPH_PATH $CHRO_SUM_QUBO
runQuboSolver $GRAPH_PATH $OUTPUT $QUBOSOLVER
