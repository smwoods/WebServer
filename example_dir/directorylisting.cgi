#!/usr/bin/perl -w

use strict;
use warnings;
use CGI;
my $cgi = CGI->new;

my $arg = "ls -la $ARGV[0]";
print "HTTP/1.1 200 OK\nContent-type: text/plain\n\n";
system($arg);