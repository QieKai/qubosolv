#!/bin/bash
function runLPSolver(){
    `echo $1 >>./graph_n_k.txt`
    i=1
    for file in ` find $1 -name "*.alist" | sort`
    do
        bfile=$(basename $file)
        n=`head -1 $file`
        #`echo [$i] ${bfile%.*} graph size n -\> $n, coloring number k -\> $n>>./graph_n_k.txt`
        `echo ${bfile%.*} $n>>./graph_n_k.txt`
        let i=i+1
    done
    `echo \ >>./graph_n_k.txt`
}

GRAPH1_PATH="./graphs1"
GRAPHS_PATH="./graphs.small"
rm -f graph_n_k.txt
runLPSolver $GRAPH1_PATH
runLPSolver $GRAPHS_PATH
