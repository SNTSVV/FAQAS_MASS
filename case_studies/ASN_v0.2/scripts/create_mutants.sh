#!/bin/bash

set -u
set -e

#export SRCIROR=/opt/srcirorfaqas
#export LLVM_BUILD=${SRCIROR}/llvm-build

#export SRCIROR_LLVMMutate_LIB=${LLVM_BUILD}/Release+Asserts/lib/LLVMMutate.so
#export SRCIROR_SRC_MUTATOR=${SRCIROR}/SRCMutation/build/mutator
#export SRCIROR_LLVM_BIN=${LLVM_BUILD}/Release+Asserts/bin/
#export SRCIROR_LLVM_INCLUDES=${LLVM_BUILD}/Release+Asserts/lib/clang/3.8.0/include/
#export SRCIROR_COVINSTRUMENTATION_LIB=${SRCIROR}/InstrumentationLib/SRCIRORCoverageLib.o

[ "${SRCIROR_SRC_MUTATOR:-}" != "" ] || { echo "SRCIROR_SRC_MUTATOR env var not set"; exit 1; }

mass_topdir=$(dirname ${SRCIROR_SRC_MUTATOR})/../../
test -d $mass_topdir || { echo "mass_topdir missing ($mass_topdir)"; exit 1; }

cd $(dirname $0)

faqas_semu_config_file=./faqas_semu_config.sh
source $faqas_semu_config_file

if test -f $FAQAS_SEMU_CASE_STUDY_WORKSPACE/faqas_semu_config.sh; then
    faqas_semu_config_file=$FAQAS_SEMU_CASE_STUDY_WORKSPACE/faqas_semu_config.sh
	source $faqas_semu_config_file
fi

#variables
SRCIROR_COMPILER=$mass_topdir/PythonWrappers/mutationClang

SRC_MUTANTS=$(readlink -fm $FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR)

mkdir -p $SRC_MUTANTS

#modify accordingly
PATH_TO_SRC=$(readlink -f $FAQAS_SEMU_REPO_ROOTDIR)

LOGFILE=$SRC_MUTANTS/mutation`date +"%Y%m%d%H%M"`.log
touch $LOGFILE

trap "exit" INT
jq -c '.[]' compile_commands.json | while read i; do
	FILE=`echo $i | jq -r '.file'`
	ARGS=`echo $i | jq '.command'`
	DIR=`echo $i | jq -r '.directory'`
	
	FILE=$(readlink -f $FILE)
	DIR=$(readlink -f $DIR)

	echo "-----------------------" 2>&1 | tee -a $LOGFILE
	echo "Mutating "$FILE 2>&1 | tee -a $LOGFILE

	cd $DIR

	path_wo_ext="${FILE%.*}"
	path_wo_root="${path_wo_ext#$PATH_TO_SRC}"
	
	mkdir -p $SRC_MUTANTS/$path_wo_root
	
	orig_dir=`dirname -- "$FILE"`

	ARGS_wo_quotes=`sed -e 's/^"//' -e 's/"$//' <<<"$ARGS"`

	echo ----------------	
	echo $ARGS_wo_quotes
	echo ---------------
		
	$SRCIROR_COMPILER --compilation "$ARGS_wo_quotes" 2>&1 | tee -a $LOGFILE
	rsync $orig_dir/*.mut.*.c $SRC_MUTANTS/$path_wo_root
	rsync $orig_dir/meta-mu.info $SRC_MUTANTS/$path_wo_root

done
