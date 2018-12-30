#!/bin/bash

set -e

LIBDIR="/usr/lib/x86_64-linux-gnu"
GLIBC="$LIBDIR/crt1.o $LIBDIR/crti.o -lc $LIBDIR/crtn.o"
DYNLINKER="-dynamic-linker /lib64/ld-linux-x86-64.so.2"

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

