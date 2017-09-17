#/bin/bash

batch_script="./simu_merge_sep3.sh"
od_prefix=$1
prefix=$2
raw_files=$3
pop_file=$4
exe_file=$5

for i in $(seq 1 10);do
	$batch_script "$od_prefix$i/" "$prefix" "$raw_files" "$pop_file" "$exe_file"
done
