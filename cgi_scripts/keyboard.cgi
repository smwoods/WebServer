#!/usr/bin/perl -w
use strict;
use warnings;
use CGI;
my $cgi = CGI->new;

system("omxplayer ~/notes/$ARGV[0]");
