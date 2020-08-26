#!/bin/bash

HOME=/home/csp
LIBCSP=$HOME/libparam/lib/libgscsp
LIBCSP_SRC=$LIBCSP/lib
LIBCSP_BUILD=$LIBCSP/build/lib/libcsp
COMPILED=libcsp.so
LIBCSP_ORIGINAL_BUILD=$LIBCSP/original_build/$COMPILED
SRC_MUTANTS=/opt/mutations/src-mutants/lib
EXEC_DIR=$HOME/test_runs
FLAG=$1
COMMAND=(./waf build)

TCE=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/tce_sha512.sh

if [ ! -f "$LIBCSP_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json.template > $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json
    cat $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json
    
    cd $LIBCSP
    ./waf configure
    ./waf clean
    ./waf build
    mkdir -p original_build
    cp $LIBCSP_BUILD/$COMPILED original_build/
fi

source $TCE $LIBCSP $LIBCSP_SRC $LIBCSP_BUILD $LIBCSP_ORIGINAL_BUILD $FLAG $SRC_MUTANTS $COMPILED $EXEC_DIR $COMMAND

