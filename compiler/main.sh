#!/bin/bash

set -e

GLIBC="/usr/lib/crt1.o /usr/lib/crti.o -lc /usr/lib/crtn.o"
DYNLINKER="-dynamic-linker /lib/ld-linux-x86-64.so.2"
SYC_CORE=$(dirname ${BASH_SOURCE[0]})/syc-core

# read arguments
INFILE=$3
OUTFILE=$2

# no input files specified?
if [ $# != 3 ]; then
    echo "Usage: $0 -o <outfile> <infile>";
    exit -1;
fi;

# get names of intermediate files
IFILE=$(basename $INFILE .sy).i
SFILE=$(basename $INFILE .sy).s
OFILE=$(basename $INFILE .sy).o

# (1) preprocess
cpp $INFILE > $IFILE

# (2) compile
$SYC_CORE $IFILE $SFILE

# (3) assemble
as -o $OFILE $SFILE

# (4) link
ld $DYNLINKER -o $OUTFILE $OFILE $GLIBC
