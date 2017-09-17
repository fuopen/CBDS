### This script is used to get the pair-wise TMRCA from psmc files.
### And make a tmrca matrix file for NJ tree.

=head2
### The format of the matix file is :
    7		## 5 char
Bovine      0.0000  1.6866  1.7198  1.6606  1.5243  1.6043  1.5905
Mouse       1.6866  0.0000  1.5232  1.4841  1.4465  1.4389  1.4629
Gibbon      1.7198  1.5232  0.0000  0.7115  0.5958  0.6179  0.5583
Orang       1.6606  1.4841  0.7115  0.0000  0.4631  0.5061  0.4710
Gorilla     1.5243  1.4465  0.5958  0.4631  0.0000  0.3484  0.3083
Chimp       1.6043  1.4389  0.6179  0.5061  0.3484  0.0000  0.2692
Human       1.5905  1.4629  0.5583  0.4710  0.3083  0.2692  0.0000
# 12 char	# 8 char
### 
=cut

### Run a command line program which could build a UPGMA tree using the distance matrix file as input. Then delete the matrix file!
## The format of output tree is:
##  [1](Bovine:0.91769,Mouse:0.76891,(Gibbon:0.35793126,(Orang:0.28469375,((Chimp:0.151675,Human:0.117525):0.03981875,Gorilla:0.15393125):0.02695625):0.04648125):0.42026874);


use strict;
my $pop = shift;
my $sample = shift;

my $mask = "../required_files/masked_regions_by_density.txt";
open MASK, $mask;

my @mask;
while (<MASK>){
	chomp;
	my @line = split;
	push @mask, [@line]
}

while (1){
	my @psmcfa = glob("../psmcfa/$pop\_$sample\_*psmcfa");
	if ($#psmcfa >= 0){
		sleep(60);
	}
	else {
		my @psmc = glob("../psmc/$pop\_$sample\_*psmc");
		last if ($#psmc == $sample * ($sample - 1) / 2 - 1);
	}
}


for (my $chr = 1; $chr <= 22; $chr++){
	open TREE, "> ../tree/$pop\_$sample\_chr$chr\.tree";
	open MASKTREE, "> ../tree/$pop\_$sample\_chr$chr\_masked.tree";
	open MTX, "> ../tree/$pop\_$sample\_chr$chr\.matrix";
	
	my (%mtx, %indis, %boundary, $print);
	my @files = glob("../psmc/$pop\_$sample\_*psmc");
	foreach my $file (@files){
		my ($indi1, $indi2, $hap1, $hap2);
		if ($file =~ /$pop\_$sample\_(.*?_\d)_(.*?_\d)\.psmc/){
			$indi1 = $1;
			$indi2 = $2;
		}
		$indis{$indi1}++;
		$indis{$indi2}++;

		open PSMC, $file;
		my ($num, $n0, @seg, $Tmax);
		while(<PSMC>){
			chomp;
			if (/RD\t20/){
				$num++;
				next;
			}
			if ($num == 1 && /TR\s+(.*?)\s+(.*)/){
				$n0 = $1 * 100000;
				next;
			}
			if ($num == 1 && /MT\s+(.*)/){
				$Tmax = $1;
				$num++;
				next;
			}
			if (/>(\d+)/){
				next if ($1 < $chr);
				last if ($1 > $chr);
				$num++;
				next;
			}
			if ($num >= 3){
				my @line = split();
				$boundary{$line[1]}++;
	
				my $time_s = int(2 * 2 * $n0 * (0.1 * exp(($line[2]/29)**2 * log(1+10*$Tmax)) - 0.1));		## interval start
				my $time_e = int(2 * 2 * $n0 * (0.1 * exp((($line[2]+1)/29)**2 * log(1+10*$Tmax)) - 0.1));	## interval end
				my $time = int(($time_s + $time_e)/2);	## middle point as the TMRCA
				$line[2] = $time;
				push @seg, [@line]
			}
		}
		$mtx{$indi1}{$indi2} = \@seg;
	}

	my @indis = keys %indis;
	my $smp_size = $#indis + 1;
		
	my ($stt, $end);
	my @bound = sort {$a <=> $b} keys %boundary;
	for (my $b = 0; $b <= $#bound; $b++){
		$print .= sprintf("%5d\n", $smp_size);
		if ($b == 0){
			$stt = 1;
		}
		else {$stt = $bound[$b-1] + 1};
		my $end = $bound[$b];
		for (my $i = 0; $i <= $#indis; $i++){
			my $indi1 = $indis[$i];
			$print .= sprintf("%-12s", $indi1);
			for (my $j = 0; $j <= $#indis; $j++){
				my $indi2 = $indis[$j];
				if ($indi1 eq $indi2){
					$print .= "0.00    ";
					next;
				}
				if ($mtx{$indi1}{$indi2}){
					for (my $seg = 0; $seg <= $#{$mtx{$indi1}{$indi2}}; $seg++){
						my $seg_s = $mtx{$indi1}{$indi2}->[$seg]->[0];
						my $seg_e = $mtx{$indi1}{$indi2}->[$seg]->[1];
						
						if ($seg_e < $stt){
							splice(@{$mtx{$indi1}{$indi2}}, 0, 1);
							$seg--;
							next;
						}
						if ($seg_s <= $stt && $seg_e >= $end){
							my $time = $mtx{$indi1}{$indi2}->[$seg]->[2];
							$print .= sprintf("%-8d", $time);
							next;
						}
						last if ($seg_s > $end);
					}
				}
				elsif ($mtx{$indi2}{$indi1}){
					for (my $seg = 0; $seg <= $#{$mtx{$indi2}{$indi1}}; $seg++){
						my $seg_s = $mtx{$indi2}{$indi1}->[$seg]->[0];
						my $seg_e = $mtx{$indi2}{$indi1}->[$seg]->[1];
	
						if ($seg_e < $stt){
							splice(@{$mtx{$indi2}{$indi1}}, 0, 1);
							$seg--;
							next;
						}
						if ($seg_s <= $stt && $seg_e >= $end){
							my $time = $mtx{$indi2}{$indi1}->[$seg]->[2];
							$print .= sprintf("%-8d", $time);
							next;
						}
						last if ($seg_s > $end);
					}
				}
				else {die "error\n"};
			}
			$print .= "\n";
		}
	}
	print MTX $print;
	system("./neighbor ../tree/$pop\_$sample\_chr$chr\.matrix ../tree/$pop\_$sample\_chr$chr\.outfile ../tree/$pop\_$sample\_chr$chr\.outtree 2");	## multiple trees
	system("rm ../tree/$pop\_$sample\_chr$chr\.matrix");
	system("rm ../tree/$pop\_$sample\_chr$chr\.outfile");

	my ($stt, $print_tree, @pos);
	my @bound = sort {$a <=> $b} keys %boundary;
	for (my $b = 0; $b <= $#bound; $b++){
		if ($b == 0){
			$stt = 1;
		}
		else {$stt = $bound[$b-1] + 1};
		my $end = $bound[$b];
		my $stt_pos = ($stt - 1) * 100 + 101;
		my $end_pos = $end * 100 + 100;

		push @pos, [$stt_pos, $end_pos];
	}

	open OUTTREE, "../tree/$pop\_$sample\_chr$chr\.outtree";
	my ($sub_tree, $cnt_tree);
	while (<OUTTREE>){
		chomp;
		$sub_tree .= $_;
		if (/;/){
			$cnt_tree++;
			$print_tree .= "[".$pos[$cnt_tree-1][0]."-".$pos[$cnt_tree-1][1]."] ".$sub_tree."\n";
			$sub_tree = ();
		}
	}

	if ($cnt_tree != $#pos + 1){
		print"\n\tError! Tree reconstruction of chr ".$chr." is failed\n";
		next;
	}
	print TREE $print_tree;


	my $mask_tree;
	open TREE, "../tree/$pop\_$sample\_chr$chr\.tree";
	while (<TREE>){
		chomp;
		my @line = split(/\t/);
		$line[0] =~ s/\[|\]//g;
		my ($stt, $end) = split(/-/, $line[0]);
	
		while ($chr > $mask[0][0]) {
			splice(@mask, 0, 1);
			last if ($#mask < 0);
		}
		while ($chr == $mask[0][0] && $stt >= $mask[0][2]){
			splice(@mask, 0, 1);
			last if ($#mask < 0);
		}
		next if ($chr == $mask[0][0] && (($stt - $mask[0][2]) * ($end - $mask[0][1]) < 0));

		$mask_tree .= $_."\n";
	}
	print MASKTREE $mask_tree;

	system("rm ../tree/$pop\_$sample\_chr$chr\.tree");
	system("rm ../tree/$pop\_$sample\_chr$chr\.outtree");
	print "\n\tTree reconstruction of chr".$chr." is done.\n";
}
