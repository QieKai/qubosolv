#!/bin/bash
function runLPSolver(){
    i=1
    for file in ` find $1 -name "*.alist" | sort`
    do
        `echo "[$i]">> $2`
        `echo ${file%.*} >> $2`
        `$3 $file >> $2`
        `echo -e "\n\n">>$2`
        let i=i+1
    done
}

LPSOLVER="./lp_solver_cplex/lp_chromatic"
GRAPH_PATH="./graphs1"
OUTPUT="out_graphs1_lp.out"
rm -f $OUTPUT
runLPSolver $GRAPH_PATH $OUTPUT $LPSOLVER
