#!/usr/bin/perl

use strict;
use warnings;
use Graphics::GnuplotIF;

my $plot = Graphics::GnuplotIF->new;

$plot->gnuplot_set_xrange( 0, 4 );
$plot->gnuplot_set_yrange( -2, 2 );
$plot->gnuplot_cmd( "set grid" );
$plot->gnuplot_plot_equation(
  "y1(x) = sin(x)",
  "y2(x) = cos(x)",
  "y3(x) = sin(x)/x" );

$plot->gnuplot_pause();