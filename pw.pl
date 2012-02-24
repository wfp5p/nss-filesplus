#! /usr/bin/perl

use strict;
use feature qw(say);
use Data::Dumper;

my @x = getpwnam('wfp5p');


say Dumper(\@x);

my @x = getpwuid(1249);

say Dumper(\@x);

__END__

setpwent();

while (my $g = getpwent()) {
    say $g;
}

endpwent();

