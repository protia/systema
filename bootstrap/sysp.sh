#!/bin/sh

set -e

# parameters
INFILE=
OUTFILE=

# loop over parametrs
for param in $*; do
	# tokenize the argument
	property=`echo $param | awk -F "=" '{printf $1}'`
	value=`echo $param | awk -F "=" '{printf $2}'`
	# process property
	if [ $property = "in" ]; then
		INFILE=$value
	elif [ $property = "out" ]; then
		OUTFILE=$value
	else
		echo "Unknown argument: $propery, ignored."
	fi
done

# no input files specified?
if [ -z "$INFILE" -o -z "$OUTFILE" ]; then
    echo "Usage: $0 in=<infile> out=<outfile>";
    exit 5;
fi;

# preprocess
m4 $INFILE > $OUTFILE
