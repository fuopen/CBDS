## This script is used to get the population size from all.psmc

use strict;

my $pop = shift;
my $sample = shift;
my $file = "../psmc/supergenome_$pop\_$sample\.psmc";
open FILE, $file;
open OUT, "> ../required_files/$pop\_$sample\_pop_size.txt";

my ($index, $n0, @last);
while (<FILE>){
	chomp;

	if (/RD\t20/){
		$index++;
	}

	if ($index >= 1 && /TR\t(.*?)\t/){
		$n0 = $1 * 100000;
		print OUT "N0\t".$n0."\n\n";
		print OUT "interval\tboundary\tlambda\n";
	}
	if ($index >=1 && /RS/){
		my @line = split;
		print OUT $line[1]."\t".$line[2]."\t".$line[3]."\n";
	}
}
