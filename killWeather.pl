#!/usr/bin/perl

use strict;

my $username = "weather";

open(data, "rsh proto.cs.earlham.edu ps auxw | grep ./getWeather|");
while(<data>) {
    my @words = reverse(split);
    while(pop(@words) ne $username) { }
    my $pid = pop(@words);
    system("rsh proto.cs.earlham.edu kill -9 $pid");
}
close(data);


exit;
