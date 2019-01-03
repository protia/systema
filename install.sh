#!/bin/sh

# Install Systema on the machine.

# Use this script if you want to install systema on UNIX

# Usage: ./install.sh <install-dir> [build-dir]

# Halt on errors
set -e

# Get Installation directory path
mkdir -p $1
INSTALL_DIR=`realpath $1`/Programs/Systema

# Get build directory path
mkdir -p $2
BUILD_DIR=`realpath $2`

# install the compiler
mkdir -p $INSTALL_DIR
: '
cp $BUILD_DIR/compiler3/macro/sysm     $INSTALL_DIR/sysm
cp $BUILD_DIR/compiler3/frontend/sysf  $INSTALL_DIR/sysf
cp $BUILD_DIR/compiler3/backend/sysb   $INSTALL_DIR/sysb
cp $BUILD_DIR/compiler3/assembler/sysa $INSTALL_DIR/sysa
cp $BUILD_DIR/compiler3/linker/sysl    $INSTALL_DIR/sysl
'
