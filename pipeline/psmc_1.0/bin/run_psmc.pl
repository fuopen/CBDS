## This script is used to get any two haplotypes from the same or different individuals.
## Change the data into psmcfa format.
#	1.	100bp per bin
#	2.	if more then 90 'N', -> 'N'
#	3.	if more then 1 heter, -> 'K'
#	4. 	else, -> 'T'

use strict;
use Storable qw(dclone);

my $path = shift;
my $pop = shift;
my $sample = shift;

if (! $sample){
	$sample = 30;	## default sample size. you may need to change this.
	print "The default sample size is 30\n\n";
}

if ($sample > 100){
	open ERR, "> ./sample_size.log";
	print ERR "***The sample size is too large, there will be too many pairs of genome.\n\n";
	print ERR "***If you want to run it using less sample size, delete this log file first.\n";
	print ERR "***If you really want to test it on larger sample, you can detele this file,\n";
	print ERR "***and modify the perl script 'run_psmc.pl' on line 20,\n";
	print ERR "***change the default limit 100 to what you want.\n";
	die("***The sample size is too large, there will be too many pairs of genome.\n***If you want to run it using less sample size, delete this log file first.\n***If you really want to test it on larger sample, you can detele this file,\n***and modify the perl script 'run_psmc.pl' on line 20,\n***change the default limit 100 to what you want.\n\n");
}

if (-e "$pop\_$sample\_sample_list.txt" && -s "$pop\_$sample\_sample_list.txt"){
	open LIST, "$pop\_$sample\_sample_list.txt";

	my @indi;
	while (<LIST>){
		chomp;
		push @indi, $_;
	}

	my $re_run = 0;
	for (my $i = 0; $i <= $#indi - 1; $i++){
		for (my $j = $i + 1; $j <= $#indi; $j++){
			my $psmc = "../psmc/$pop\_$sample\_$indi[$i]\_$indi[$j]\.psmc";
			next if (-e $psmc && -s $psmc);
			$re_run++;
		}
	}
	if ($re_run == 0){
		die("\n\tpsmc files are here\n\tYou have already run this script. Skip this part.\n\n");
	}
}

my $file = "$path\/$pop\_chr1.snps.genotypes.sd";
open FILE, $file;
open SAMPLE, "> ./$pop\_$sample\_sample_list.txt";
	
my (%sample, @sample, $sum);
while (<FILE>){
	chomp;
	
	my @line = split;
	while (1){
		my $indi = int(rand($#line));
	
		if ($indi >= 6 && $indi <= $#line){
			next if ($sample{$line[$indi]});
			$sample{$line[$indi]} = $indi + 1;
			push @sample, $line[$indi];
			print SAMPLE $line[$indi]."\n";
			$sum++;
		}
			
		last if ($sum >= $sample);	
	}
	last;
}
print "\n\tSampling of ".$sample." haploid genome is done\n";


my %ref;
for (my $chr = 1; $chr <= 22; $chr++){
	my $ref = "../ref_genome/human_g1k_v37_chr$chr\_masked.fasta";
	open REF, $ref;

	while (<REF>){
		chomp;
		my @line = split(//);
		for (my $i = 0; $i<= $#line; $i++){
			push @{$ref{$chr}}, $line[$i];
		}
	}
}

for (my $i = 0; $i <= $#sample - 1; $i++){
	for (my $j = $i + 1; $j <= $#sample; $j++){
		my $indi1 = $sample[$i];
		my $indi2 = $sample[$j];
		my $idx1 = $sample{$indi1};
		my $idx2 = $sample{$indi2};
		open OUT, "> ../psmcfa/$pop\_$sample\_$indi1\_$indi2\.psmcfa";

		for (my $chr = 1; $chr <= 22; $chr++){
			print OUT ">".$chr;
			my @ref_rep = @{dclone(\@{$ref{$chr}})};
			
			`awk -v idx1=$idx1 -v idx2=$idx2 '\$idx1 != \$idx2 {print \$2}' $path\/$pop\_chr$chr\.snps.genotypes.sd > $path\/$pop\_$indi1\_$indi2\.diff`;
			open DIFF, "$path\/$pop\_$indi1\_$indi2\.diff";
			while (<DIFF>){
				chomp;
				next if (/POS/);

				my $seg = int($_/100);	## bin index
				if ($ref_rep[$seg] eq 'T'){
					$ref_rep[$seg] = 'K';
				}
				elsif (! $ref_rep[$seg]){
					$ref_rep[$seg] = 'K';
				}
			}

			my $print .= "\n".$ref_rep[0];
			for (my $l = 1; $l <= $#ref_rep; $l++){
				if ($l%60==0){
					$print .= "\n";
				}
				$print .= $ref_rep[$l];
			}
			$print .= "\n";
			print OUT $print;

			system("rm $path\/$pop\_$indi1\_$indi2\.diff");
		}

		system("bsub -M 20000 -W 24:00 sh submit_psmc.sh ../psmc/$pop\_$sample\_$indi1\_$indi2\.psmc ../psmcfa/$pop\_$sample\_$indi1\_$indi2\.psmcfa");
		print "\n\tPSMC of ".$indi1." and ".$indi2." is submited to the cluster\n";
	}
}
