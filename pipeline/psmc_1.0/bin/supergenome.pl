## This script is used to get two haplotypes from the same individuals.
## Change the data into psmcfa format.
#	1.	100bp per bin
#	2.	if more then 90 'N', -> 'N'
#	3.	if more then 1 heter, -> 'K'
#	4. 	else, -> 'T'

use strict;
use Storable qw(dclone);

my $path = shift;
my $pop = shift;
my $sample = shift;		## dipliod genomes

if (! $sample){
	$sample = 10;	## default sample size. you may need to change this.
	print "The default sample size is 10\n\n";
}

if ($sample > 30){
	open ERR, "> ./sample_size.log";
	print ERR "***The sample size is too large, it will take too long to run psmc.\n\n";
	print ERR "***If you want to run it using less sample size, delete this log file first.\n";
	print ERR "***If you really want to test it on larger sample, you can detele this file,\n";
	print ERR "***and modify the perl script 'mix_hap.pl' on line 20,\n";
	print ERR "***change the default limit 30 to what you want.\n";
	die("***The sample size is too large, there will be too many pairs of genome.\n***If you want to run it using less sample size, delete this log file first.\n***If you really want to test it on larger sample, you can detele this file,\n***and modify the perl script 'mix_hap.pl' on line 20,\n***change the default limit 30 to what you want.\n\n");
}


my $file = "$path\/$pop\_chr1.snps.genotypes.sd";
open FILE, $file;
open SAMPLE, "> ./$pop\_$sample\_for_supergenome.txt";
open OUT, "> ../psmcfa/supergenome_$pop\_$sample\.psmcfa";
	
my (%sample, @sample, $sum);
while (<FILE>){
	chomp;
	
	my @line = split;
	while (1){
		if (($#line - 5)/2 <= $sample){
			my $new_sample = ($#line - 5)/2;
			print "\n\tThe sequenced genomes are less than ".$sample.".\nUse all ".$new_sample." genomes\n";
			for (my $i = 6; $i <= $#line; $i++){
				$line[$i] =~ s/_1|_2//;
				push @sample, $line[$i];
				$i++;
			}
			last;
		}
		
		my $indi = int(rand($#line));
		if ($indi >= 6 && $indi <= $#line){
			$line[$indi] =~ s/_1|_2//;
			next if ($sample{$line[$indi]});
			$sample{$line[$indi]} = int(($indi-6)/2) + 1;
			push @sample, $line[$indi];
			print SAMPLE $line[$indi]."\n";
			$sum++;
		}
			
		if ($sum >= $sample){
			print "\n\tSampling of ".$sample." diploid genome is done\n";
			last;
		}
	}
	last;
}


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

my $chr_all = 0;
for (my $i = 0; $i <= $#sample; $i++){
	my $indi = $sample[$i];
	my $idx1 = $sample{$indi} * 2 + 4;
	my $idx2 = $sample{$indi} * 2 + 5;

	for (my $chr = 1; $chr <= 22; $chr++){
		$chr_all++;
		print OUT ">".$chr_all;
		my @ref_rep = @{dclone(\@{$ref{$chr}})};
			
		`awk -v idx1=$idx1 -v idx2=$idx2 '\$idx1 != \$idx2 {print \$2}' $path\/$pop\_chr$chr\.snps.genotypes.sd > $path\/$pop\_$indi\.diff`;
		open DIFF, "$path\/$pop\_$indi\.diff";
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

		system("rm $path\/$pop\_$indi\.diff");
	}
}

system("bsub -q shared -M 20000 -W 24:00 sh submit_supergenome.sh ../psmc/supergenome_$pop\_$sample\.psmc ../psmcfa/supergenome_$pop\_$sample\.psmcfa");
print "\n\tPSMC of $sample\ supergenome is submited to the cluster\n";
