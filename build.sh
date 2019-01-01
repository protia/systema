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
COMPILER1_BUILD=$BUILD_DIR/compiler1
COMPILER2_BUILD=$BUILD_DIR/compiler2
COMPILER3_BUILD=$BUILD_DIR/compiler3

# Build bootstrap compiler
echo "*** PASS 0: Build bootstrap compiler ***";
for component in $COMPONENTS; do

        # src: <src-dir>/bootstrap/frontend
	SRC_SUBDIR=$BOOTSTRAP_SRC/$component

	# dest: <build-dir>/bootstrap/frontend
	BUILD_SUBDIR=$BOOTSTRAP_BUILD/$component

        # make build directory
	mkdir -p $BUILD_SUBDIR

        # build steps depend on the kind of component
        if [ $component = frontend ]; then

		LIST=`cd $SRC_SUBDIR; ls *.c | awk -F "\\." '{print $1}'`

        	for fname in $LIST; do
			# files
			cfile="$SRC_SUBDIR/$fname".c
			ofile="$BUILD_SUBDIR/$fname".o
			# steps
			echo "[+] Compiling: $cfile"
			gcc -c -o $ofile $cfile
		done

                ofiles=`ls $BUILD_SUBDIR/*.o`
		pro_file="$BUILD_SUBDIR/sysf"

		gcc -o $pro_file $ofiles

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
echo "*** PASS 1: Use bootstrap compiler to compile compiler1 ***" ;
CPATH=$BOOTSTRAP_BUILD
DPATH=$COMPILER1_BUILD
for component in $COMPONENTS; do

        # src: <src-dir>/compiler/frontend
	SRC_SUBDIR=$COMPILER_SRC/$component

	# dest: <build-dir>/compiler/frontend
	BUILD_SUBDIR=$DPATH/$component

        # make build directory
	mkdir -p $BUILD_SUBDIR

        # build steps depend on the kind of component
        if [ $component = frontend ]; then

		# commands
		SYSP=$CPATH/preproc/sysp
		SYSF=$CPATH/frontend/sysf
		SYSB=$CPATH/backend/sysb
		SYSA=$CPATH/assembler/sysa
		SYSL=$CPATH/linker/sysl

		LIST=`cd $SRC_SUBDIR; ls *.sys | awk -F "\\." '{print $1}'`

        	for fname in $LIST; do
			# files
			sys_file="$SRC_SUBDIR/$fname".sys
			pre_file="$BUILD_SUBDIR/$fname".pre
			int_file="$BUILD_SUBDIR/$fname".int
			asm_file="$BUILD_SUBDIR/$fname".asm
			obj_file="$BUILD_SUBDIR/$fname".obj
			# steps
			echo "[+] Compiling: $sys_file"
			$SYSP inc=$SRC_SUBDIR in=$sys_file out=$pre_file
			$SYSF in=$pre_file out=$int_file
			$SYSB in=$int_file out=$asm_file
			$SYSA in=$asm_file out=$obj_file
		done

                obj_files=`ls $BUILD_SUBDIR/*.obj | awk '{print "in="$0}'`
		pro_file="$BUILD_SUBDIR/sysf"

		$SYSL out=$pro_file $obj_files

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

# Use the compiler to compile itself
echo "*** PASS 2: Use compiler1 to compile compiler2 ***" ;
CPATH=$COMPILER1_BUILD
DPATH=$COMPILER2_BUILD
for component in $COMPONENTS; do

        # src: <src-dir>/compiler/frontend
	SRC_SUBDIR=$COMPILER_SRC/$component

	# dest: <build-dir>/compiler/frontend
	BUILD_SUBDIR=$DPATH/$component

        # make build directory
	mkdir -p $BUILD_SUBDIR

        # build steps depend on the kind of component
        if [ $component = frontend ]; then

		# commands
		SYSP=$CPATH/preproc/sysp
		SYSF=$CPATH/frontend/sysf
		SYSB=$CPATH/backend/sysb
		SYSA=$CPATH/assembler/sysa
		SYSL=$CPATH/linker/sysl

		LIST=`cd $SRC_SUBDIR; ls *.sys | awk -F "\\." '{print $1}'`

        	for fname in $LIST; do
			# files
			sys_file="$SRC_SUBDIR/$fname".sys
			pre_file="$BUILD_SUBDIR/$fname".pre
			int_file="$BUILD_SUBDIR/$fname".int
			asm_file="$BUILD_SUBDIR/$fname".asm
			obj_file="$BUILD_SUBDIR/$fname".obj
			# steps
			echo "[+] Compiling: $sys_file"
			$SYSP inc=$SRC_SUBDIR in=$sys_file out=$pre_file
			$SYSF in=$pre_file out=$int_file
			$SYSB in=$int_file out=$asm_file
			$SYSA in=$asm_file out=$obj_file
		done

                obj_files=`ls $BUILD_SUBDIR/*.obj | awk '{print "in="$0}'`
		pro_file="$BUILD_SUBDIR/sysf"

		$SYSL out=$pro_file $obj_files

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

# Use the compiler to compile itself
echo "*** PASS 3: Use compiler2 to compile compiler3 ***" ;
CPATH=$COMPILER2_BUILD
DPATH=$COMPILER3_BUILD
for component in $COMPONENTS; do

        # src: <src-dir>/compiler/frontend
	SRC_SUBDIR=$COMPILER_SRC/$component

	# dest: <build-dir>/compiler/frontend
	BUILD_SUBDIR=$DPATH/$component

        # make build directory
	mkdir -p $BUILD_SUBDIR

        # build steps depend on the kind of component
        if [ $component = frontend ]; then

		# commands
		SYSP=$CPATH/preproc/sysp
		SYSF=$CPATH/frontend/sysf
		SYSB=$CPATH/backend/sysb
		SYSA=$CPATH/assembler/sysa
		SYSL=$CPATH/linker/sysl

		LIST=`cd $SRC_SUBDIR; ls *.sys | awk -F "\\." '{print $1}'`

        	for fname in $LIST; do
			# files
			sys_file="$SRC_SUBDIR/$fname".sys
			pre_file="$BUILD_SUBDIR/$fname".pre
			int_file="$BUILD_SUBDIR/$fname".int
			asm_file="$BUILD_SUBDIR/$fname".asm
			obj_file="$BUILD_SUBDIR/$fname".obj
			# steps
			echo "[+] Compiling: $sys_file"
			$SYSP inc=$SRC_SUBDIR in=$sys_file out=$pre_file
			$SYSF in=$pre_file out=$int_file
			$SYSB in=$int_file out=$asm_file
			$SYSA in=$asm_file out=$obj_file
		done

                obj_files=`ls $BUILD_SUBDIR/*.obj | awk '{print "in="$0}'`
		pro_file="$BUILD_SUBDIR/sysf"

		$SYSL out=$pro_file $obj_files

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

# Compile the samples using last compiler built
echo "*** build the samples using compiler 3 ***"
CPATH=$COMPILER3_BUILD
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
        # print progress
	echo "[+] Compiling: $sys_file"
	# steps
	$SYSP in=$sys_file out=$pre_file
	$SYSF in=$pre_file out=$int_file
	$SYSB in=$int_file out=$asm_file
	$SYSA in=$asm_file out=$obj_file
	$SYSL in=$obj_file out=$pro_file
	$pro_file > $out_file
done
