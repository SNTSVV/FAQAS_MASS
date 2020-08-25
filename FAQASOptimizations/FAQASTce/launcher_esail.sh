#!/bin/bash

HOME=/home/svf
ESAIL=$HOME/Obsw/Source
ESAIL_SRC=$ESAIL
ESAIL_BUILD=$ESAIL/_binaries
COMPILED=OBSW.exe
ESAIL_ORIGINAL_BUILD=$ESAIL/original_build/$COMPILED
SRC_MUTANTS=/opt/mutations/src-mutants
EXEC_DIR=$HOME/test_runs
FLAG=$1
COMMAND=(make target release=true)

TCE=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/tce_sha512.sh

if [ ! -f "$ESAIL_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" $ESAIL/Makefile.template > $ESAIL/Makefile
    cat $ESAIL/Makefile
 
    cd $ESAIL
    make clean
    make target release=true
    mkdir -p original_build
    cp $ESAIL_BUILD/$COMPILED $ESAIL/original_build/
fi

source $TCE $ESAIL $ESAIL_SRC $ESAIL_BUILD $ESAIL_ORIGINAL_BUILD $FLAG $SRC_MUTANTS $COMPILED $EXEC_DIR $COMMAND

