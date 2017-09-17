## This script is used to mask the reference genome.

use strict;

my $mask = "../required_files/masked_regions_by_density.txt";

my $re_run = 0;		## check if there are masked ref genome
for (my $chr = 1; $chr <= 22; $chr++){
	my $masked_ref = "../ref_genome/human_g1k_v37_chr17_masked.fasta";
	next if (-e $masked_ref && -s $masked_ref);
	$re_run++;
}
if ($re_run == 0){
	die("\n\tMasked refrence genome is here.\n\n");
}

for (my $chr = 1; $chr <= 22; $chr++){
	my @mask;
	open MASK, $mask;
	while (<MASK>){
		chomp;
		next if (/chr/);
		my @line = split;
		if ($line[0] == $chr){
			push @mask, [$line[1], $line[2]];
		}	
	}

	system("samtools faidx ../ref_genome/human_g1k_v37.fasta $chr\ > ../ref_genome/human_g1k_v37_chr$chr\.fasta");
	open FASTA, "../ref_genome/human_g1k_v37_chr$chr\.fasta";
	open NEW, "> ../ref_genome/human_g1k_v37_chr$chr\_masked.fasta";

	my ($pos, $N, @ref);
	while (<FASTA>){
		chomp;
		next if (/^>/);

		my @line = split(//);
		for (my $i = 0; $i <= $#line; $i++){
			$pos++;
			if ($pos >= $mask[0][0] && $pos <= $mask[0][1]){
				$N++;
			}
			if ($pos > $mask[0][1]){
				splice(@mask, 0, 1);
			}

			if ($pos%100 == 0){
				if ($N >= 90){
					push @ref, 'N';
				}
				else{push @ref, 'T'};
				$N = 0;
			}
		}
	}

	my $seq;
	for (my $i = 0; $i <= $#ref; $i++){
		if ($i == 0){
			$seq .= $ref[$i];
			next;
		}
		if ($i%60==0){
			$seq .= "\n";
		}
		$seq .= $ref[$i];
		if ($i == $#ref){
			$seq .= "\n";
		}
	}
	print NEW $seq;

	system("rm ../ref_genome/human_g1k_v37_chr$chr\.fasta");
	print "\n\tMasking the reference genome of chr$chr"." is done.";
}
