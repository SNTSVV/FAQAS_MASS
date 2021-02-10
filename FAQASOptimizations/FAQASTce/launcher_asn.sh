#!/bin/bash

HOME=/home/asn
GRAMMAR=$HOME/mutation_test
GRAMMAR_SRC=$GRAMMAR
GRAMMAR_BUILD=$GRAMMAR
COMPILED=test.o
GRAMMAR_ORIGINAL_BUILD=$GRAMMAR/original_build/$COMPILED
SRC_MUTANTS=/opt/mutations/src-mutants
EXEC_DIR=$HOME/tce_test_runs
FLAG=$1
COMMAND=(make)

TCE=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/tce_sha512.sh

if [ ! -f "$GRAMMAR_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" $GRAMMAR/Makefile.template > $GRAMMAR/Makefile
    cat $GRAMMAR/Makefile
    
    cd $GRAMMAR
    make clean
    make
    mkdir -p original_build
    cp $COMPILED original_build/
fi

source $TCE $GRAMMAR $GRAMMAR_SRC $GRAMMAR_BUILD $GRAMMAR_ORIGINAL_BUILD $FLAG $SRC_MUTANTS $COMPILED $EXEC_DIR $COMMAND

