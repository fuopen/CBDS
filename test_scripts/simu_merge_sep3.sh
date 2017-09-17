#!/bin/bash
############################################
#code segment0:
#	for display the usage of this script
#show the complete manual
############################################
if test $# -ne 5;then
	echo "incorrect number of arguments"
	echo "usage:"
	echo '$1' should be the output directory
	echo '$2' should be the output prefix,\
		e.g CHB,CEU,YRI
	echo '$3' should be the input of the raw\
		files
	echo '$4' should be the population size\
		file
	echo '$5' should be the execute file
	exit 1
fi
############################################
#code segment1:
#	for merge files and restore the filename
# and file lengths
############################################
if test -e "$1";then
	dir=$1
else
	mkdir $1
	dir=$1
fi
outprefix=$2
list_files=($(ls $3|sed -n /^"$outprefix.*tree$"/s#^#"$3"#gp));
storefiles=()
file_range=()

for((count=0;count<${#list_files[@]};count++));do
	files=${list_files[$count]}
	if [ $count -gt 30 ]; then
		break
	fi
	if [ -f $files ]; then
			#echo $files
			cat $files >> "$dir$outprefix.tree"
			flen_tmp=$(awk 'END{ print NR}' $files)
			file_range=(${file_range[@]} $flen_tmp)
			storefiles=(${storefiles[@]} $files)
	else continue
	fi	
done
#echo ${file_range[@]} > "$dir${outprefix}.range"
#echo ${storefiles[@]} > "$dir${outprefix}.files"
###########################################
#code segment2:
#	for generate res alp and tree file for
#estimating key parameters
###########################################

psize_file=$4
exe_file=$5
$exe_file "-t" "$dir$outprefix.tree" "-d" $psize_file "-o" "$dir" "-p" $outprefix -m
rm "$dir$outprefix.tree"
