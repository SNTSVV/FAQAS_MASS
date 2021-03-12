#!/bin/bash

# set SRCIROR path
export SRCIROR=
                                                     
export LLVM_BUILD=${SRCIROR}/llvm-build
export SRCIROR_LLVMMutate_LIB=${LLVM_BUILD}/Release+Asserts/lib/LLVMMutate.so
export SRCIROR_SRC_MUTATOR=${SRCIROR}/SRCMutation/build/mutator
export SRCIROR_LLVM_BIN=${LLVM_BUILD}/Release+Asserts/bin/
export SRCIROR_LLVM_INCLUDES=${LLVM_BUILD}/Release+Asserts/lib/clang/3.8.0/include/
export SRCIROR_COVINSTRUMENTATION_LIB=${SRCIROR}/InstrumentationLib/SRCIRORCoverageLib.o
export SRCIROR_COMPILER=$SRCIROR/PythonWrappers/mutationClang
export MASS=$SRCIROR/MASS

####

export HOME=

# set directory path where MASS files can be stored
# example: APP_RUN_DIR=/opt/example
export APP_RUN_DIR=

# specify if MASS will be executed on a HPC, possible values are "true" or "false"
# example: HPC="true"
export HPC=

# directory root path of the software under test
# example: PROJ=$HOME/mlfs
export PROJ=

# directory src path of the SUT
# example: PROJ_SRC=$PROJ/libm
export PROJ_SRC=

# directory test path of the SUT
# example: PROJ_TST=$HOME/unit-test-suite
export PROJ_TST=

# directory path of the compiled binary
# example: PROJ_BUILD=$PROJ/build-host/bin
export PROJ_BUILD= 

# filename of the compiled file/library                                                                                     
# example: COMPILED=libmlfs.a
export COMPILED=

# path to original Makefile
# example: ORIGINAL_MAKEFILE=$PROJ/Makefile
export ORIGINAL_MAKEFILE=

# compilation command of the SUT
# example: COMPILATION_CMD=(make all ARCH=host EXTRA_CFLAGS="-DNDEBUG" \&\& make all COVERAGE="true" ARCH=host_cov EXTRA_CFLAGS="-DNDEBUG")
export COMPILATION_CMD=

# compilation additional commands of the SUT (e.g., setup of workspace)
# example ADDITIONAL_CMD=(cd $HOME/blts/BLTSConfig \&\& make clean install INSTALL_PATH="$HOME/blts_install" \&\& cd $HOME/blts_workspace \&\& $HOME/blts_install/bin/blts_app --init)
export ADDITIONAL_CMD=

# command to be executed after each test case (optional)
# example: ADDITIONAL_CMD_AFTER=(rm -rf $HOME/blts_workspace/*)
export ADDITIONAL_CMD_AFTER=

# compilation command for TCE analysis
# example TCE_COMPILE_CMD=(make all ARCH=host EXTRA_CFLAGS="-DNDEBUG")
export TCE_COMPILE_CMD=

# command to clean installation of the SUT
# example CLEAN_CMD=(make cleanall)
export CLEAN_CMD=

# relative path to location of gcov files (i.e., gcda and gcno files)
# example GC_FILES_RELATIVE_PATH=Reports/Coverage/Data
export GC_FILES_RELATIVE_PATH=

### MASS variables

# TCE flags to be tested 
#example FLAGS=("-O0" "-O1" "-O2" "-O3" "-Ofast" "-Os")
export FLAGS=

# set if MASS should be executed with a prioritized and reduced test suite
# example PRIORITIZED="true"
export PRIORITIZED=

# set sampling technique, possible values are "uniform", "stratified", and "fsci"
# note: if "uniform" or "stratified" is set, $PRIORITIZED must be "false"
# example SAMPLING="fsci"
export SAMPLING=

# set sampling rate if whether "uniform" or "stratified" sampling has been selected
#example RATE="0.003"
export RATE=

####

export MUTANTS_DIR=$APP_RUN_DIR/src-mutants
mkdir -p $MUTANTS_DIR

export MUTATION_DIR=$APP_RUN_DIR/MUTATION
export COV_FILES=$APP_RUN_DIR/COV_FILES


