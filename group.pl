#! /usr/bin/perl

use strict;
use feature qw(say);
use Data::Dumper;

my @x = getgrnam('g_imed_hr');

say Dumper(\@x);

my @x = getgrgid(13242);

say Dumper(\@x);

__END__

setgrent();

while (my $g = getgrent()) {
    say $g;
}

endgrent();


