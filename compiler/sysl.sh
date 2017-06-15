#!/bin/bash

set -e

GLIBC="/usr/lib/crt1.o /usr/lib/crti.o -lc /usr/lib/crtn.o"
DYNLINKER="-dynamic-linker /lib/ld-linux-x86-64.so.2"

# read arguments
INFILE=$3
OUTFILE=$2

# no input files specified?
if [ $# != 3 ]; then
    echo "Usage: $0 -o <outfile> <infile>";
    exit -1;
fi;

# link
ld $DYNLINKER -o $OUTFILE $INFILE $GLIBC
