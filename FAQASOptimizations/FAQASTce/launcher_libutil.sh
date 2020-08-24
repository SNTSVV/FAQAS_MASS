#!/bin/bash

HOME=/home/gsl
LIBUTIL=$HOME/libutil
LIBUTIL_SRC=$LIBUTIL/src
LIBUTIL_BUILD=$LIBUTIL/build
COMPILED=libgsutil_cmocka.a
LIBUTIL_ORIGINAL_BUILD=$LIBUTIL/original_build/$COMPILED
SRC_MUTANTS=/opt/mutations/src-mutants
EXEC_DIR=$HOME/test_runs
FLAG="-O0"
COMMAND="./waf build"

TCE=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/tce_diff.sh

if [ ! -f "$LIBUTIL_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" $LIBUTIL/tools/buildtools/gs/buildtools/compiler_settings.json.template > $LIBUTIL/tools/buildtools/gs/buildtools/compiler_settings.json
    cat $LIBUTIL/tools/buildtools/gs/buildtools/compiler_settings.json
    
    cd $LIBUTIL
    ./waf configure
    ./waf clean
    ./waf build
    mkdir -p original_build
    cp build/$COMPILED original_build/
fi

source $TCE $LIBUTIL $LIBUTIL_SRC $LIBUTIL_BUILD $LIBUTIL_ORIGINAL_BUILD $FLAG $SRC_MUTANTS $COMPILED $EXEC_DIR $COMMAND

