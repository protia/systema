#!/bin/bash

SYSC_CORE=$(dirname ${BASH_SOURCE[0]})/sysc-core

set -e

# read arguments
INFILE=$3
OUTFILE=$2

# no input files specified?
if [ $# != 3 ]; then
    echo "Usage: $0 -o <outfile> <infile>";
    exit -1;
fi;

# compile
$SYSC_CORE $INFILE $OUTFILE

