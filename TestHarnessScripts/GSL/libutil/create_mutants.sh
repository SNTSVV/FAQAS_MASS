#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

export SRCIROR=/opt/srcirorfaqas
export LLVM_BUILD=${SRCIROR}/llvm-build

export SRCIROR_LLVMMutate_LIB=${LLVM_BUILD}/Release+Asserts/lib/LLVMMutate.so
export SRCIROR_SRC_MUTATOR=${SRCIROR}/SRCMutation/build/mutator
export SRCIROR_LLVM_BIN=${LLVM_BUILD}/Release+Asserts/bin/
export SRCIROR_LLVM_INCLUDES=${LLVM_BUILD}/Release+Asserts/lib/clang/3.8.0/include/
export SRCIROR_COVINSTRUMENTATION_LIB=${SRCIROR}/InstrumentationLib/SRCIRORCoverageLib.o

#variables
SRCIROR_COMPILER=/opt/srcirorfaqas/PythonWrappers/mutationClang
SRC_MUTANTS=/opt/mutations/src-mutants

LOGFILE=$SRC_MUTANTS/mutation`date +"%Y%m%d%H%M"`.log
touch $LOGFILE

trap "exit" INT
jq -c '.[]' compile_commands.json | while read i; do
	FILE=$(echo $i | jq -r '.file')
	ARGS=$(echo $i | jq '.arguments')
	DIR=$(echo $i | jq -r '.directory')
	echo "-------------------------- " 2>&1 | tee -a $LOGFILE
	echo "Mutating "$FILE 2>&1 | tee -a $LOGFILE

	FLAGS=""
	
	for row in $(echo "${ARGS}" | jq -r '.[] '); do
		FLAGS+="$row "
	done
	
	cd $DIR	
	
	absolute_path=$(readlink -m ${FILE})
 	path_wo_ext="${absolute_path%.*}"	
	path_wo_root="${path_wo_ext#/opt/libutil/src/}"	

	echo mkdir -p $SRC_MUTANTS/$path_wo_root
	
	orig_dir=$(dirname -- "$absolute_path")

	echo $SRCIROR_COMPILER $FLAGS 2>&1 | tee -a $LOGFILE
        echo mv $orig_dir/*.mut.*.c $SRC_MUTANTS/$path_wo_root
	break
done
