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
list_files=($(ls $3|sed -n /^"$outprefix"/s#^#"$3"#gp));
storefiles=()
file_range=()

for((count=0;count<${#list_files[@]};count++));do
	files=${list_files[$count]}
	if [ $count -gt 100 ]; then
		break
	fi
	if [ -f $files ]; then
		echo $files
		cat $files >> "$dir$outprefix.tree"
		flen_tmp=$(awk 'END{ print NR}' $files)
		file_range=(${file_range[@]} $flen_tmp)
		storefiles=(${storefiles[@]} $files)
	else continue
	fi	
done

echo ${file_range[@]} > "$dir${outprefix}.range"
echo ${storefiles[@]} > "$dir${outprefix}.files"
###########################################
#code segment2:
#	for generate res alp and tree file for
#estimating key parameters
###########################################

psize_file=$4
exe_file=$5
$exe_file "-t" "$dir$outprefix.tree" "-d" $psize_file "-o" "$dir" "-p" $outprefix -m
##########################################
#code segment3:
#	for seperate the files and extract the
# merge file to each seperate files
###########################################

resfile=$(ls $dir|sed -n /_res/s#^#"$dir"#gp)
alpfile=$(ls $dir|sed -n /_alp/s#^#"$dir"#gp)
treefile=$(ls $dir|sed -n /_tree/s#^#"$dir"#gp)
Treefile=$(ls $dir|sed -n /_Tree/s#^#"$dir"#gp)

rangefile=$(ls $dir|sed -n /range/s#^#"$dir"#gp)
storefile=$(ls $dir|sed -n /files/s#^#"$dir"#gp)

filelist=($(cat $storefile))
lenglist=($(cat $rangefile))

file_start=1
file_end=0

if [ -e "${dir}res/" ];then
	dir_res="${dir}res/"
else
	mkdir "${dir}res/"
	dir_res="${dir}res/"
fi

if [ -e "${dir}alp/" ];then
	dir_alp="${dir}alp/"
else
	mkdir "${dir}alp/"
	dir_alp="${dir}alp/"
fi

if [ -e "${dir}tree/" ];then
	dir_tree="${dir}tree/"
else
	mkdir "${dir}tree/"
	dir_tree="${dir}tree/"
fi

if [ -e "${dir}Tree/" ];then
	dir_Tree="${dir}Tree/"
else
	mkdir "${dir}Tree/"
	dir_Tree="${dir}Tree/"
fi

for((i=0;i<${#filelist[@]};i++));do
	file=${filelist[$i]}
	outres=$(echo $file|sed "s#^\/.*\/\([^.]*\)\.tree#"$dir_res"\1_res.txt#")
	outalp=$(echo $file|sed "s#^\/.*\/\([^.]*\)\.tree#"$dir_alp"\1_alp.txt#")
	outtree=$(echo $file|sed "s#^\/.*\/\([^.]*\)\.tree#"$dir_tree"\1_tree.txt#")
	outTree=$(echo $file|sed "s#^\/.*\/\([^.]*\)\.tree#"$dir_Tree"\1_Tree.txt#")
	#echo $outres $outalp $outtree
	if [ -f $file ];then
		len=${lenglist[$i]}
		file_end=$(($file_end+$len))
		sed -n "$file_start,${file_end}p" "$resfile" > $outres
		sed -n "$file_start,${file_end}p" "$alpfile" > $outalp
		sed -n "$file_start,${file_end}p" "$treefile"> $outtree
		sed -n "$file_start,${file_end}p" "$Treefile"> $outTree
		file_start=$(($file_start+$len))
	fi
done
