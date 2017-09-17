mkdir ../ref_genome
mkdir ../psmcfa
mkdir ../psmc
mkdir ../tree
mkdir ../required_files


## make sure there are reference genome files in this folder, otherwise download them
if [ -f ../ref_genome/human_g1k_v37.fasta ]
then 
	echo '
	human_g1k_v37.fasta is here'
else 
	if [ -f ../ref_genome/human_g1k_v37.fasta.gz ]
	then
		echo '
		human_g1k_v37.fasta.gz is here'
	else
		echo '
		human_g1k_v37.fasta.gz is not here'
		echo '
			Downloading the human reference genome build37
			'
		wget -q -P ../ref_genome/ ftp://ftp.1000genomes.ebi.ac.uk/vol1/ftp/technical/reference/human_g1k_v37.fasta.gz
	fi
		
	echo '
		Extracting the reference sequence
		'
	gunzip -c ../ref_genome/human_g1k_v37.fasta.gz > ../ref_genome/human_g1k_v37.fasta
fi


	
if [ -f ../ref_genome/human_g1k_v37.fasta.fai ]
then 
	echo '
	human_g1k_v37.fasta.fai is here'
else 
	echo '
	human_g1k_v37.fasta.fai is not here'
	echo '
		Downloading the human reference genome build37 fai file
		'
	wget -q -P ../ref_genome/ ftp://ftp.1000genomes.ebi.ac.uk/vol1/ftp/technical/reference/human_g1k_v37.fasta.fai
fi


## make sure the mased regions file is here
if [ -f ../required_files/masked_regions_by_density.txt ]
then
	echo '
	masked_regions_by_density.txt is here'
	echo '
	Masking the refenrence genome according to the mask regions released by 1000 Genomes Project...
	'
	perl mk_mask_ref_fasta.pl
else
	echo '
		No file is named masked_regions_by_density.txt
	'
	##wget -q -P ../required_files/ http://the_url_of_our_files....
	exit 0
fi


## converge the study genome sequences into .psmcfa format
## And run psmc for each pair of haplotypes.

if [ ! $1 ]
then
	echo '
	Error! You need to specify a path of your study genome data!'
	exit 0
else
	echo '
	Your path of genome data is:' $1
fi

if [ ! $2 ]
then
	echo '
	Error! You need to specify a population!'
	exit 0
else
	echo '
	Your study population is:' $2
fi

if [ ! $3 ]
then
	echo '
	Sampling 30 haplotypes randomly by default'
else
	echo '
	Your study sample size is:' $3
fi

if [ -f $1\/$2\_chr1.snps.genotypes.sd ]
then
	echo '
	genome sequence data is here'
else 
	echo '
	Error! genome sequence data is not here!'
	exit 0
fi

if [ -f sample_size.log ]
then
	echo '
	Error! You need to delete sample_size.log before re-run it.'
	exit 0
fi

perl run_psmc.pl $1 $2 $3


## reconstruct the TMRCA tree

sleep 1800
echo '
	Reconstruct the coalescent trees.'
perl consensus_tmrca_tree.pl $2 $3




echo '
	Done
	'
