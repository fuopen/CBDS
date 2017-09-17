#######################################################################
#######################################################################
author: Sile Hu
Date  : 2014-04-01

#######################################################################
This software is used to detect different types of selection, 
e.g positive,negative and balancing selection and reconstruct the 
coalescent tree by correct raw tree datas. It can be used to calculate 
summary statistcs generated from our methods and produce several sepcific files:

files with different postfix:
_res : include result of summary statistics, H_1 and H_2 for each block;
_alp : include each time intervals rescaled alpha values for each block;
_tree: include corrected time intervals for each block;
_Tree: include corrected TREE (parenthetics format) for each block;
_rat : include alp_correct ratios for total trees in each file


usage:
the program can accept any specific parameters or their combination,
and different types of option with '-', of which details as following:
	-t: tree format files (parenthetic format);
	-d:	demographic trajectory file;
	-r: alpla ratio file;
	-o: output directory or output files;
	-p: prefix of the output files if specified -o option
	-i: time interval file;
	-m: if output ratio file;
	-v: print the version of current software

details: (program name is psmc_opt in default, here use "prog"
#############################################################################
a). ./prog -t **.tree -d **pop_size.txt
	calculate the summary statistics for each tree of block, and produce 
	files with postfix with default name "PSMC_res,PSMC_alp,PSMC_tree,
	PSMC_Tree" in current direcotry;

b). ./prog -t **.tree -d **pop_size.txt -o /home/someone/
	specify the output files names with "PSMC_res, PSMC_alp,PSMC_tree,
	PSMC_Tree" in directory "/home/someone/"
c). ./prog -t **.tree -d **pop_size.txt -o /home/someone/ -p my_psmc
	specify the output files with name "/home/someone/my_psmc_res.txt,
	/home/someone/my_psmc_alp.txt,/home/someone/my_psmc_tree.txt,/home/
	someone/my_psmc_Tree.txt"
d). ./prog -t **.tree -d **pop_size.txt -r alpha_ratio.txt
	the same as case a), just except the correction is specified by alpha
	 ratio file "alpha_ratio.txt" in current directory
e). ./prog -t **.tree -d **pop -r alpha_ratio -o /home/someone/
	the same as b), just except that add alpha ratio file "PSMC_RATIO.
	txt" in "/home/someone/"
f). ./prog -t **.tree -d **pop -r alpha_ratio -o /home/someone/ -p my_psmc
	like case c), just add ratio file with prefix "/home/someone/my_psmc"

	all case above can add -m option to produce alpha ratio file to see the
	total alpha ratio
#############################################################################
#############################################################################
g). ./prog -t **.tree -i **_tree.txt
	just update raw tree **.tree with corrected time interval file **_tree
	.txt, output file named "PSMC_UPDATE.tree" in current directory
h). ./prog -t **.tree -i **_tree.txt -o /home/someone/out_tree.txt
	like case g) but specify the output file name out_tree.txt in /home/someone/
i). ./prog -t **.tree
	just resovle **.tree and output time interval file "PSMC_TREE_INTV.txt" in
	current directory
j). ./prog -t **.tree -o /home/someone/tree_time_interval.txt
	like case i) just except that specify the output file name "/home/someone/tree
	_time_interval.txt"
#############################################################################
#############################################################################
k). ./prog -v
	print the version of prog
#############################################################################
