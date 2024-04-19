#!/usr/bin/perl
use strict;
use warnings;

sub rtrim {
    my $line = $_[0];
    $line =~ s/\s+$//;
    return $line;
}

sub load_table {
    open my $file, '<', $_[0];
    my @table = ();
    while (my $line = <$file>) {
        push(@table, rtrim($line));
    }
    return @table;
}

sub is_valid {
    my $result = $#_ > 0 && length($_[0]) > 0;
    for (my $i = 1; $i < $#_ && $result; $i++) {
        $result &= length($_[$i]) == length($_[$i - 1]);
    }
    return $result;
}

sub find_in_line {
    my $i = index($_[0], $_[1]);
    if ($i >= 0) {
        return ($i, 0);
    }
    $i = index($_[0], reverse($_[1]));
    if ($i >= 0) {
        return ($i + length($_[1]) - 1, 1);
    }
    return (-1, 0);
}

sub find_in_table {
    my $text = $_[0];
    my @table = @_[1..$#_];

    for (my $y = 0; $y < $#table; $y++) {
        my ($i, $reverse) = find_in_line($table[$y], $text);
        if ($i >= 0) {
            return ($y, $i, $reverse ? "Left" : "Right");
        }
    }
    for (my $x = 0; $x < length($table[0]); $x++) {
        my $column = "";
        for (my $y = 0; $y < $#table; $y++) {
            $column .= substr($table[$y], $x, 1);
        }
        my ($i, $reverse) = find_in_line($column, $text);
        if ($i >= 0) {
            return ($i, $x, $reverse ? "Up" : "Down");
        }
    }
    for (my $y = 0; $y < $#table; $y++) {
        my $diagonal = "";
        for (my $x = 0; $x < length($table[0]) && $y + $x < $#table; $x++) {
            $diagonal .= substr($table[$y + $x], $x, 1);
        }
        my ($i, $reverse) = find_in_line($diagonal, $text);
        if ($i >= 0) {
            return ($y + $i, $i, $reverse ? "Up-left" : "Down-right");
        }
    }
    for (my $x = 0; $x < length($table[0]); $x++) {
        my $diagonal = "";
        for (my $y = 0; $x + $y < length($table[0]) && $y < $#table; $y++) {
            $diagonal .= substr($table[$y], $x + $y, 1);
        }
        my ($i, $reverse) = find_in_line($diagonal, $text);
        if ($i >= 0) {
            return ($i, $x + $i, $reverse ? "Up-left" : "Down-right");
        }
    }
    for (my $y = 0; $y < $#table; $y++) {
        my $diagonal = "";
        for (my $x = 0; $x < length($table[0]) && $y - $x >= 0; $x++) {
            $diagonal .= substr($table[$y - $x], $x, 1);
        }
        my ($i, $reverse) = find_in_line($diagonal, $text);
        if ($i >= 0) {
            return ($y - $i, $i, $reverse ? "Down-left" : "Up-right");
        }
    }
    for (my $x = 0; $x < length($table[0]); $x++) {
        my $diagonal = "";
        for (my ($ix, $y) = ($x, $#table - 1); $ix < length($table[0]) && $y >= 0; $ix++, $y--) {
            $diagonal .= substr($table[$y], $ix, 1);
        }
        my ($i, $reverse) = find_in_line($diagonal, $text);
        if ($i >= 0) {
            return (length($table[0]) - 1 - $i, $x + $i, $reverse ? "Down-left" : "Up-right");
        }
    }
    return (0, 0, "Not found");
}

sub print_table {
	foreach my $line (@_) {
        print "$line\n";
    }
}

if ($#ARGV) {
    die "Error: Needs a file name passed as a command-line argument\n";
}

my @table = load_table($ARGV[0]);
if (!is_valid(@table)) {
    die "Error: Table for word search must be a non-empty rectangle\n";
}

print_table @table;
print "Enter searched word: ";
while (my $input = <STDIN>) {
    my ($y, $x, $direction) = find_in_table(rtrim($input), @table);
    print "row: " . ($y + 1) . "\ncolumn: " . ($x + 1) . "\ndirection: $direction\n";
    print "Enter searched word: ";
}
