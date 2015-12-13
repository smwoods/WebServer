#!/usr/bin/perl -w

use strict;
use warnings;
use CGI;
my $cgi = CGI->new;


if(!defined $ARGV[0]){
    print "No FilePath Specified!\n";
}


if(!defined $ARGV[1]){
    print "No FilePath Specified!\n";
}

# my $num_args = $#ARGV + 1;
# printf("we have %d arguments\n", $num_args);
# printf("first agument: %s\n", $ARGV[0]);

my $filename = "img_dir/$ARGV[0]-output.gif";
my $data = "data_dir/$ARGV[0]-output.dat";

open GNUPLOT,"|gnuplot --persist";

my $parameter = <<GNU;
set title "$ARGV[0]"
set boxwidth 0.5
set style fill solid
set yrange [0:$ARGV[1]]
set terminal gif enhanced
set output "$filename"
plot "$data" using 1:3:xtic(2) with boxes
GNU

print GNUPLOT $parameter;


# set key left box
# set samples 50
# set style data points
# set terminal gif
# set output $filename
# plot [-10:10] sin(x),atan(x),cos(atan(x))