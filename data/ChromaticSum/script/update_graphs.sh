#!/bin/bash
function updateK(){
  IFS=' '
  ary=($1)
  #for key in "${!ary[@]}"; do echo "$key ${ary[$key]}"; done
  filename="$2/${ary[0]}.H"
  #echo $filename
  line="${ary[1]} ${ary[2]}"
  sed -i '' -e "1s/.*/$line/" $filename
}

function update_graphs(){
while IFS='' read -r line || [[ -n "$line" ]]; do
      updateK "$line" "$GRAPH1_PATH"

done < $1
}
GRAPH1_PATH="./graphs1"
update_graphs "graph_n_k.txt"
