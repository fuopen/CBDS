### This script is used to change the primary psmc file which contain TMRCA info into 
### smaller file.
### The adjacent segments which coalescent at same time will be compressed to one segment.


use strict;

my $psmc_file = shift;
open PSMC, $psmc_file;

my $new_psmc_file = $psmc_file."2";
open NEW, ">".$new_psmc_file;

my $num = 0;
my (@tmrca, $chr, $print);
while (<PSMC>){
	chomp;

	if (/TMRCA/){
		$chr++;
		next;
	}
	
	if (/^\d/){
			my @line = split(/\s+/);
			for (my $i = 0; $i <= $#line; $i++){
				push @{$tmrca[$chr]}, $line[$i];	
			}
	}

	else {
		$print .= $_."\n";
	}
}

for (my $chr = 1; $chr <= 22; $chr++){
	$print .= ">".$chr."\n";
	my ($stt, $end, %segs);
	$stt = 1;
	for (my $i = 2; $i <= $#{$tmrca[$chr]}; $i++){
		if ($tmrca[$chr][$i] ne $tmrca[$chr][$i-1]){
			$end = $i - 1;
			$segs{$stt}{'end'} = $end;
			$segs{$stt}{'tmrca'} = $tmrca[$chr][$i-1];

			$stt = $i;
		}
	}

	$segs{$stt}{'end'} = $#{$tmrca[$chr]}; 
	$segs{$stt}{'tmrca'} = $tmrca[$chr][$#{$tmrca[$chr]}];

	foreach my $stt (sort {$a <=> $b} keys %segs){
		$print .= $stt."\t".$segs{$stt}{'end'}."\t".$segs{$stt}{'tmrca'}."\n";
	}
}

print NEW $print;
system("mv $new_psmc_file $psmc_file");
