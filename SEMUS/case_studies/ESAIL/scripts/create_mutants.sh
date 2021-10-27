#!/bin/bash

set -u
set -e

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

ARGS='-fdata-sections -ffunction-sections -Wall -DASW_VERSION_MAJOR=4 -DASW_VERSION_MINOR=1 -DSVN_REVISION=0 -O0 -g -DTARGET -mcpu=v8 -I_Ext/check_for_target/include/ -DLOG_WCET -DSCHEDULER_TRACKING -DFLIGHT_RELEASE -I./Source/. -I./Source/ApplicationLayer/. -I./Source/ServiceLayer/. -I./Source/HighLevelDriverLayer/. -I./Source/ProtocolLayer/. -I./Source/LowLevelDriverLayer/. -I./Source/Utilities/. -I/opt/rtems-4.8-SAT-AIS/sparc-rtems4.8/leon3/lib/include -I./Source/_Ext/mlfs/include -DEDISOFT'

src_filename=$(basename -s .c $FAQAS_SEMU_ORIGINAL_SOURCE_FILE)
src_filename_o=$src_filename.o

echo "-----------------------" 2>&1 | tee -a $LOGFILE
echo "Mutating ./$ENV_FAQAS_SEMU_SRC_FILE" 2>&1 | tee -a $LOGFILE

FILE=$(realpath "$FAQAS_SEMU_ORIGINAL_SOURCE_FILE")
pushd $FAQAS_SEMU_REPO_ROOTDIR

path_wo_ext="${FILE%.*}"
path_wo_root="${path_wo_ext#$PATH_TO_SRC}"
	
mkdir -p $SRC_MUTANTS/$path_wo_root
	
$SRCIROR_COMPILER --compilation "-c -o _objects/$src_filename_o ./$ENV_FAQAS_SEMU_SRC_FILE $ARGS" 2>&1 | tee -a $LOGFILE
popd

orig_dir=$(dirname -- "$FAQAS_SEMU_ORIGINAL_SOURCE_FILE")

echo "rsync $orig_dir/*.mut.*.c $SRC_MUTANTS/$path_wo_root"
echo "rsync -av --no-p --no-g --remove-source-files $orig_dir/*.mut.*.c $SRC_MUTANTS/$path_wo_root"
rsync -av --no-p --no-g --remove-source-files $orig_dir/*.mut.*.c $SRC_MUTANTS/$path_wo_root
rsync -av --no-p --no-g --remove-source-files $orig_dir/meta-mu.info $SRC_MUTANTS/$path_wo_root

cd - > /dev/null

