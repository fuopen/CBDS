./psmc -N20 -t15 -r5 -p "20*1+5*2" -o $1 $2
rm $2
perl psmc2seg.pl $1
