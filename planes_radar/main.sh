#!/bin/bash
outfile=test.log
tim=10s
while true;
	do
		timeout $tim nc localhost 30003 > $outfile
		msg_count=$(cat $outfile | wc -l)
		echo "We got $msg_count messages for last 40 sec"
		./plot.R
		rm $outfile
		echo "Iteration done!"
	done
