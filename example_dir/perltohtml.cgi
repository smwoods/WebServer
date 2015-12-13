#!/usr/bin/perl
use strict;
use warnings;
open FILE, '<', "example_dir/sampledata.txt" or die $!;
my $total = 0;
while (<FILE>) {
    my ($value) = split;
    $total += $value;
}
print "<html><head><title>SUM</title></head><body>The sum of sampledata.txt is " . $total . "</body>"