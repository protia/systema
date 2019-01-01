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

# compiler components
COMPONENTS="preproc frontend backend assembler linker"

# Get source directory path
SRC_DIR=`realpath $(dirname $0)`

# Get build directory path
mkdir -p $1
BUILD_DIR=`realpath $1`

# subdirectories
BOOTSTRAP_SRC=$SRC_DIR/bootstrap
BOOTSTRAP_BUILD=$BUILD_DIR/bootstrap
COMPILER_SRC=$SRC_DIR/compiler
COMPILER_BUILD=$BUILD_DIR/compiler

# Build bootstrap compiler
for component in $COMPONENTS; do

        # src: <src-dir>/bootstrap/frontend
	SRC_SUBDIR=$BOOTSTRAP_SRC/$component

	# dest: <build-dir>/bootstrap/frontend
	BUILD_SUBDIR=$BOOTSTRAP_BUILD/$component

        # make build directory
	mkdir -p $BUILD_SUBDIR

        # build steps depend on the kind of component
        if [ $component = frontend ]; then
		gcc -o $BUILD_SUBDIR/sysf $SRC_SUBDIR/*.c
        elif [ $component = preproc ]; then
		cp $SRC_SUBDIR/sysp.sh $BUILD_SUBDIR/sysp
	elif [ $component = backend ]; then
		cp $SRC_SUBDIR/sysb.sh $BUILD_SUBDIR/sysb
	elif [ $component = assembler ]; then
		cp $SRC_SUBDIR/sysa.sh $BUILD_SUBDIR/sysa
	elif [ $component = linker ]; then
		cp $SRC_SUBDIR/sysl.sh $BUILD_SUBDIR/sysl
        fi

done

# Use the bootstrap compiler to build our compiler
CPATH=$BOOTSTRAP_BUILD
for component in $COMPONENTS; do

        # src: <src-dir>/compiler/frontend
	SRC_SUBDIR=$COMPILER_SRC/$component

	# dest: <build-dir>/compiler/frontend
	BUILD_SUBDIR=$COMPILER_BUILD/$component

        # make build directory
	mkdir -p $BUILD_SUBDIR

        # build steps depend on the kind of component
        if [ $component = frontend ]; then

		LIST=`cd $SRC_SUBDIR; ls *.sys | awk -F "\\." '{print $1}'`

        	for fname in $LIST; do
			# files
			sys_file="$SRC_SUBDIR/$fname".sys
			pre_file="$BUILD_SUBDIR/$fname".pre
			int_file="$BUILD_SUBDIR/$fname".int
			asm_file="$BUILD_SUBDIR/$fname".asm
			obj_file="$BUILD_SUBDIR/$fname".obj
			pro_file="$BUILD_SUBDIR/$fname".pro
			out_file="$BUILD_SUBDIR/$fname".out
			# commands
			SYSP=$CPATH/preproc/sysp
			SYSF=$CPATH/frontend/sysf
			SYSB=$CPATH/backend/sysb
			SYSA=$CPATH/assembler/sysa
			SYSL=$CPATH/linker/sysl
			# steps
			echo "[+] Compiling: $sys_file"
			$SYSP inc=$SRC_SUBDIR in=$sys_file out=$pre_file
			$SYSF in=$pre_file out=$int_file
			$SYSB in=$int_file out=$asm_file
			$SYSA in=$asm_file out=$obj_file
		done

		cp $CPATH/frontend/sysf $BUILD_SUBDIR/sysf

        elif [ $component = preproc ]; then
		cp $SRC_SUBDIR/sysp.sh $BUILD_SUBDIR/sysp
	elif [ $component = backend ]; then
		cp $SRC_SUBDIR/sysb.sh $BUILD_SUBDIR/sysb
	elif [ $component = assembler ]; then
		cp $SRC_SUBDIR/sysa.sh $BUILD_SUBDIR/sysa
	elif [ $component = linker ]; then
		cp $SRC_SUBDIR/sysl.sh $BUILD_SUBDIR/sysl
        fi
done

# Compile the samples
CPATH=$COMPILER_BUILD
for sample in $SAMPLES; do
	mkdir -p $BUILD_DIR/samples
	# files
	sys_file="$SRC_DIR"/samples/"$sample".sys
	pre_file="$BUILD_DIR"/samples/"$sample".pre
	int_file="$BUILD_DIR"/samples/"$sample".int
	asm_file="$BUILD_DIR"/samples/"$sample".asm
	obj_file="$BUILD_DIR"/samples/"$sample".obj
	pro_file="$BUILD_DIR"/samples/"$sample".pro
	out_file="$BUILD_DIR"/samples/"$sample".out
	# commands
        SYSP=$CPATH/preproc/sysp
        SYSF=$CPATH/frontend/sysf
        SYSB=$CPATH/backend/sysb
        SYSA=$CPATH/assembler/sysa
        SYSL=$CPATH/linker/sysl
	# steps
	$SYSP in=$sys_file out=$pre_file
	$SYSF in=$pre_file out=$int_file
	$SYSB in=$int_file out=$asm_file
	$SYSA in=$asm_file out=$obj_file
	$SYSL in=$obj_file out=$pro_file
	$pro_file > $out_file
done
