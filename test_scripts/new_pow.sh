#!/bin/bash
if test $# -ne 5; then
	printf "error input %d arguments: you shoule print 6 arguments!" $# 
	echo "usage:"
	echo '$1:directory of the tree_files'
	echo '$2:directory of the pop_size_file'
	#echo '$3:prefix of the poplation\
	#		example CHB CEU'
	echo '$3:output directory'
	echo '$4:scale file'
	echo '$5:execute file'
	exit 1
fi

dir=$1
pop_file=$2
#prefix=$3
if test -e $3;then
	output=$3
else
	mkdir $3
	output=$3
fi
scale_file=$4
exe_file=$5

output=$(echo $output|sed 's![0-9a-zA-Z]$!&/!')

if test -e "${output}res";then
	outres="${output}res/"
else
	mkdir "${output}res"
	outres="${output}res/"
fi

if test -e "${output}alp";then
	outalp="${output}alp/"
else
	mkdir "${output}alp"
	outalp="${output}alp/"
fi

if test -e "${output}tree";then
	outtree="${output}tree/"
else
	mkdir "${output}tree"
	outtree="${output}tree/"
fi

if test -e "${output}Tree";then
	outTree="${output}Tree"
else
	mkdir "${output}Tree"
	outTree="${output}Tree"
fi

outres=$(echo $outres|sed 's![0-9a-zA-Z]$!&/!')
outalp=$(echo $outalp|sed 's![0-9a-zA-Z]$!&/!')
outtree=$(echo $outtree|sed 's![0-9a-zA-Z]$!&/!')
outTree=$(echo $outTree|sed 's![0-9a-zA-Z]$!&/!')
count=0
#for k in `ls $dir|grep $prefix`; do
for k in `ls $dir`; do
	j=$dir$k
	tmp=${k%.*}
	#tmp_out=$out_put$tmp
	count=$((count+1))
	u=$((count%150))
	$exe_file "-t" $j "-d" $pop_file "-r" $scale_file "-o" $output "-p" $tmp 
	tmp_out=$output$tmp
	mv "${tmp_out}_res.txt" $outres
	mv "${tmp_out}_alp.txt" $outalp
	mv "${tmp_out}_tree.txt" $outtree
	mv "${tmp_out}_Tree.txt" $outTree
	if test $u -eq 0 ; then
		sleep 1
	fi
done
