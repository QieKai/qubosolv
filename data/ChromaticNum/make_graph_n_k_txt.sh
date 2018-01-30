#!/bin/bash
function runLPSolver(){
    #`echo $1 >>./graph_n_k.txt`
    i=1
    for file in ` find $1 -name "*.alist" | sort`
    do
        bfile=$(basename $file)
        n=`head -1 $file`
        k=`$2 $file`
        N=$(($n*$n))
        #`echo [$i] ${bfile%.*} graph size n -\> $n, coloring number k -\> $n>>./graph_n_k.txt`
        `echo ${bfile%.*} $N $k>>./graph_n_k.txt`

        let i=i+1
    done
    `echo \ >>./graph_n_k.txt`
}

LPSOLVER="./lp_solver_cplex/lp_chromatic"
GRAPH1_PATH="./graphs1"
GRAPHS_PATH="./graphs.small"
rm -f graph_n_k.txt
runLPSolver $GRAPH1_PATH $LPSOLVER
#runLPSolver $GRAPHS_PATH
