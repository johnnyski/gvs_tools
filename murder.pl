#! /usr/local/bin/perl

#
# Kill PID and all children.
#
sub kill_me_and_my_kids {
	local($pid, @pslist) = @_;
	local(@list);
	local($i);

	foreach (@pslist) {
		split;
		$list[$i++] = "$_[$c1]" if $_[$c2] == $pid;
	}

	kill 9, $pid;

	if ($#list >= 0) {
		foreach $p (@list) {
			kill_me_and_my_kids($p, @pslist);
		}
	}
	
	return;
}


if ($#ARGV != 0) {
	print "Usage: $0 pid\n";
	exit(0);
}
$pid = $ARGV[0];

$opsys = `uname`;

if ($opsys == "Linux") {
	$c1 = 2;
	$c2 = 3;
	$pscmd = "ps al";
} elsif ($opsys == "HP-UX") {
	$c1 = 1;
	$c2 = 2;
	$pscmd = "ps -ef";
}
open(PS, "$pscmd |");
while(<PS>) {
	next if /COMMAND/;
	$pslist[$i++] = $_;
}
close PS;


&kill_me_and_my_kids($pid, @pslist);
