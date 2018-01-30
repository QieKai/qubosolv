#!/bin/bash
function chromaticSumQUBO(){
    for file in ` ls $1 `
    do
        if [ -d $1"/"$file ]
        then
             chromaticSumQUBO $1"/"$file
        else
             `./chromatic_qubo $1"/"$file `
        fi
    done
}

function runQuboSolver(){
    i=1
	for file in ` find $1 -name "*.H"`
	do
        `echo "[$i]">>./run_graphs_small_BB.out`
		`echo ${file%.*} >>./run_graphs_small_BB.out`
		`./quboSolver_BB/quboSolver $file >>./run_graphs_small_BB.out`
		`echo -e "\n\n">>./run_graphs_small_BB.out`
        let i=i+1
	done
}

GRAPH_PATH="./graphs.small"
rm -f $GRAPH_PATH/*.H
rm -f run_graphs_small_BB.out
./update_graphs.sh
chromaticSumQUBO $GRAPH_PATH
runQuboSolver $GRAPH_PATH
