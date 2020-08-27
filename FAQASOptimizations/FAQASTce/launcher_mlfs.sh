#!/bin/bash

HOME=/home/mlfs
MLFS=$HOME/mlfs
MLFS_SRC=$MLFS/libm
MLFS_BUILD=$MLFS/build-host/bin
COMPILED=libmlfs.a
MLFS_ORIGINAL_BUILD=$MLFS/original_build/$COMPILED
SRC_MUTANTS=/opt/mutations/src-mutants
EXEC_DIR=$HOME/test_runs
FLAG=$1
COMMAND=(make all ARCH=host EXTRA_CFLAGS="-DNDEBUG")

TCE=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/tce_sha512.sh

if [ ! -f "$MLFS_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" $MLFS/Makefile.template > $MLFS/Makefile
    cat $MLFS/Makefile
    
    cd $MLFS
    make clean
   	make all ARCH=host EXTRA_CFLAGS="-DNDEBUG"
    mkdir -p original_build
    cp $MLFS_BUILD/$COMPILED $MLFS/original_build
fi

source $TCE $MLFS $MLFS_SRC $MLFS_BUILD $MLFS_ORIGINAL_BUILD $FLAG $SRC_MUTANTS $COMPILED $EXEC_DIR $COMMAND

