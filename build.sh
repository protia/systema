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
COMPONENTS="utils,none macro,sysm frontend,sysf backend,sysb assembler,sysa linker,sysl"

# Get source directory path
TOP_SRC_DIR=`realpath $(dirname $0)`

# Get build directory path
mkdir -p $1
TOP_BUILD_DIR=`realpath $1`

# subdirectories
COMPILER0_BUILD=$TOP_BUILD_DIR/compiler0
COMPILER1_BUILD=$TOP_BUILD_DIR/compiler1
COMPILER2_BUILD=$TOP_BUILD_DIR/compiler2
COMPILER3_BUILD=$TOP_BUILD_DIR/compiler3

# Build bootstrap compiler
echo "*** PASS 0: Build bootstrap compiler using host compiler ***";
SPATH=$TOP_SRC_DIR/bootstrap
DPATH=$COMPILER0_BUILD

for component in $COMPONENTS; do

        # get short and long name
        LNAME=`echo $component | awk -F"," '{print $1}'`
        SNAME=`echo $component | awk -F"," '{print $2}'`

        # src & dest dir
	SRC_SUBDIR=$SPATH/$LNAME
	BUILD_SUBDIR=$DPATH/$LNAME

        # make build directory
	mkdir -p $BUILD_SUBDIR

        # get list of subfiles
	LIST=`cd $SRC_SUBDIR; ls *.c | awk -F "\\." '{print $1}'`

        # compile each file
	for fname in $LIST; do
		# files
		cfile="$SRC_SUBDIR/$fname".c
		ofile="$BUILD_SUBDIR/$fname".o
		# steps
		echo "[+] Compiling: $cfile"
		gcc -c -o $ofile $cfile
	done

        # link all the files together
        if [ $SNAME != none ]; then
		obj_list=`ls $BUILD_SUBDIR/*.o`
		exe_file="$BUILD_SUBDIR/$SNAME"
		gcc -o $exe_file $obj_list
	fi

done

# Use the bootstrap compiler to build our compiler
echo "*** PASS 1: Compile compiler1 ***" ;
SPATH=$TOP_SRC_DIR/compiler
CPATH=$COMPILER0_BUILD
DPATH=$COMPILER1_BUILD
for component in $COMPONENTS; do

        # get short and long name
        LNAME=`echo $component | awk -F"," '{print $1}'`
        SNAME=`echo $component | awk -F"," '{print $2}'`

        # src & dest dir
	SRC_SUBDIR=$SPATH/$LNAME
	BUILD_SUBDIR=$DPATH/$LNAME

        # make build directory
	mkdir -p $BUILD_SUBDIR

	# commands
	SYSM=$CPATH/macro/sysm
	SYSF=$CPATH/frontend/sysf
	SYSB=$CPATH/backend/sysb
	SYSA=$CPATH/assembler/sysa
	SYSL=$CPATH/linker/sysl

        # get list of subfiles
	LIST=`cd $SRC_SUBDIR; ls *.sys | awk -F "\\." '{print $1}'`

        # compile each file
	for fname in $LIST; do
		# files
		sys_file="$SRC_SUBDIR/$fname".sys
		pre_file="$BUILD_SUBDIR/$fname".pre
		int_file="$BUILD_SUBDIR/$fname".int
		asm_file="$BUILD_SUBDIR/$fname".asm
		obj_file="$BUILD_SUBDIR/$fname".obj
		# steps
		echo "[+] Compiling: $sys_file"
		$SYSM in=$sys_file out=$pre_file inc=$SPATH/utils inc=$SRC_SUBDIR
		$SYSF in=$pre_file out=$int_file
		$SYSB in=$int_file out=$asm_file
		$SYSA in=$asm_file out=$obj_file
	done

        # link all the files together
        if [ $SNAME != none ]; then
	 	if [ $LNAME = frontend ]; then
		        obj_list=`ls $BUILD_SUBDIR/*.obj $DPATH/utils/*.obj | awk '{print "in="$0}'`
	        else
	        	obj_list=`ls $BUILD_SUBDIR/*.obj | awk '{print "in="$0}'`
	        fi
		pro_file="$BUILD_SUBDIR/$SNAME"
		$SYSL out=$pro_file $obj_list
	fi;
done

: '

# Use the compiler to compile itself
echo "*** PASS 2: Use compiler1 to compile compiler2 ***" ;
SPATH=$TOP_SRC_DIR/compiler
CPATH=$COMPILER1_BUILD
DPATH=$COMPILER2_BUILD
for component in $COMPONENTS; do

        # get short and long name
        LNAME=`echo $component | awk -F"," '{print $1}'`
        SNAME=`echo $component | awk -F"," '{print $2}'`

        # src & dest dir
	SRC_SUBDIR=$SPATH/$LNAME
	BUILD_SUBDIR=$DPATH/$LNAME

        # make build directory
	mkdir -p $BUILD_SUBDIR

	# commands
	SYSM=$CPATH/macro/sysm
	SYSF=$CPATH/frontend/sysf
	SYSB=$CPATH/backend/sysb
	SYSA=$CPATH/assembler/sysa
	SYSL=$CPATH/linker/sysl

        # get list of subfiles
	LIST=`cd $SRC_SUBDIR; ls *.sys | awk -F "\\." '{print $1}'`

        # compile each file
	for fname in $LIST; do
		# files
		sys_file="$SRC_SUBDIR/$fname".sys
		pre_file="$BUILD_SUBDIR/$fname".pre
		int_file="$BUILD_SUBDIR/$fname".int
		asm_file="$BUILD_SUBDIR/$fname".asm
		obj_file="$BUILD_SUBDIR/$fname".obj
		# steps
		echo "[+] Compiling: $sys_file"
		$SYSM in=$sys_file out=$pre_file inc=$SPATH/utils
		$SYSF in=$pre_file out=$int_file
		$SYSB in=$int_file out=$asm_file
		$SYSA in=$asm_file out=$obj_file
	done

        # link all the files together
        if [ $SNAME -ne none ]; then
		obj_list=`ls $BUILD_SUBDIR/*.obj | awk '{print "in="$0}'`
		pro_file="$BUILD_SUBDIR/$SNAME"
		$SYSL out=$pro_file $obj_list
	fi
done

# Use the compiler to compile itself
echo "*** PASS 3: Use compiler2 to compile compiler3 ***" ;
SPATH=$TOP_SRC_DIR/compiler
CPATH=$COMPILER2_BUILD
DPATH=$COMPILER3_BUILD
for component in $COMPONENTS; do

        # get short and long name
        LNAME=`echo $component | awk -F"," '{print $1}'`
        SNAME=`echo $component | awk -F"," '{print $2}'`

        # src & dest dir
	SRC_SUBDIR=$SPATH/$LNAME
	BUILD_SUBDIR=$DPATH/$LNAME

        # make build directory
	mkdir -p $BUILD_SUBDIR

	# commands
	SYSM=$CPATH/macro/sysm
	SYSF=$CPATH/frontend/sysf
	SYSB=$CPATH/backend/sysb
	SYSA=$CPATH/assembler/sysa
	SYSL=$CPATH/linker/sysl

        # get list of subfiles
	LIST=`cd $SRC_SUBDIR; ls *.sys | awk -F "\\." '{print $1}'`

        # compile each file
	for fname in $LIST; do
		# files
		sys_file="$SRC_SUBDIR/$fname".sys
		pre_file="$BUILD_SUBDIR/$fname".pre
		int_file="$BUILD_SUBDIR/$fname".int
		asm_file="$BUILD_SUBDIR/$fname".asm
		obj_file="$BUILD_SUBDIR/$fname".obj
		# steps
		echo "[+] Compiling: $sys_file"
		$SYSM in=$sys_file out=$pre_file
		$SYSF in=$pre_file out=$int_file
		$SYSB in=$int_file out=$asm_file
		$SYSA in=$asm_file out=$obj_file
	done

        # link all the files together
        obj_list=`ls $BUILD_SUBDIR/*.obj | awk '{print "in="$0}'`
	pro_file="$BUILD_SUBDIR/$SNAME"
	$SYSL out=$pro_file $obj_list

done
'

# Compile the samples using last compiler built
echo "*** build the samples using compiler 3 ***"
CPATH=$COMPILER1_BUILD
for sample in $SAMPLES; do
	mkdir -p $TOP_BUILD_DIR/samples
	# files
	sys_file="$TOP_SRC_DIR"/samples/"$sample".sys
	pre_file="$TOP_BUILD_DIR"/samples/"$sample".pre
	int_file="$TOP_BUILD_DIR"/samples/"$sample".int
	asm_file="$TOP_BUILD_DIR"/samples/"$sample".asm
	obj_file="$TOP_BUILD_DIR"/samples/"$sample".obj
	pro_file="$TOP_BUILD_DIR"/samples/"$sample".pro
	out_file="$TOP_BUILD_DIR"/samples/"$sample".out
	# commands
        SYSM=$CPATH/macro/sysm
        SYSF=$CPATH/frontend/sysf
        SYSB=$CPATH/backend/sysb
        SYSA=$CPATH/assembler/sysa
        SYSL=$CPATH/linker/sysl
        # print progress
	echo "[+] Compiling: $sys_file"
	# steps
	$SYSM in=$sys_file out=$pre_file
	$SYSF in=$pre_file out=$int_file

	continue
	$SYSB in=$int_file out=$asm_file
	$SYSA in=$asm_file out=$obj_file
	$SYSL in=$obj_file out=$pro_file
	$pro_file > $out_file
done
