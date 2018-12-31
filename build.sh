#!/bin/sh

# Build Systema for the following target:
# OS=UNIX
# ARCH=x86
# FORMAT=ELF

# Use this script if you want to install systema on UNIX

# Usage: ./build.sh [build-dir]

# Halt on errors
set -e

# samples
SAMPLES=hello

# Get source directory path
SRC_DIR=`dirname $0`

# Get build directory path
mkdir -p $1
BUILD_DIR=`realpath $1`

# Build bootstrap compiler
mkdir -p $BUILD_DIR/bootstrap
gcc -o $BUILD_DIR/bootstrap/sysf $SRC_DIR/bootstrap/*.c
cp $SRC_DIR/bootstrap/sysp.sh $BUILD_DIR/bootstrap/sysp
cp $SRC_DIR/bootstrap/sysb.sh $BUILD_DIR/bootstrap/sysb
cp $SRC_DIR/bootstrap/sysa.sh $BUILD_DIR/bootstrap/sysa
cp $SRC_DIR/bootstrap/sysl.sh $BUILD_DIR/bootstrap/sysl

# Build the pre-processor
mkdir -p $BUILD_DIR/sysp
cp $BUILD_DIR/bootstrap/sysp $BUILD_DIR/sysp/sysp

# Build the front end
mkdir -p $BUILD_DIR/sysf
cp $BUILD_DIR/bootstrap/sysf $BUILD_DIR/sysf/sysf

# Build the back end
mkdir -p $BUILD_DIR/sysb
cp $BUILD_DIR/bootstrap/sysb $BUILD_DIR/sysb/sysb

# Build the assembler
mkdir -p $BUILD_DIR/sysa
cp $BUILD_DIR/bootstrap/sysa $BUILD_DIR/sysa/sysa

# Build the linker
mkdir -p $BUILD_DIR/sysl
cp $BUILD_DIR/bootstrap/sysl $BUILD_DIR/sysl/sysl

# Compile the samples
mkdir -p $BUILD_DIR/samples
for sample in $SAMPLES; do
	sys_file="$SRC_DIR"/samples/"$sample".sys
	pre_file="$BUILD_DIR"/samples/"$sample".pre
	int_file="$BUILD_DIR"/samples/"$sample".int
	asm_file="$BUILD_DIR"/samples/"$sample".asm
	obj_file="$BUILD_DIR"/samples/"$sample".obj
	pro_file="$BUILD_DIR"/samples/"$sample".pro
	out_file="$BUILD_DIR"/samples/"$sample".out
	$BUILD_DIR/sysp/sysp in=$sys_file out=$pre_file
	$BUILD_DIR/sysf/sysf in=$pre_file out=$int_file
	$BUILD_DIR/sysb/sysb in=$int_file out=$asm_file
	$BUILD_DIR/sysa/sysa in=$asm_file out=$obj_file
	$BUILD_DIR/sysl/sysl in=$obj_file out=$pro_file
	$pro_file > $out_file
done
