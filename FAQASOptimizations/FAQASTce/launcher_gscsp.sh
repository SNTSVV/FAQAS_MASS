#!/bin/bash

HOME=/home/csp
LIBGSCSP=$HOME/libparam/lib/libgscsp
LIBGSCSP_SRC=$LIBGSCSP/src
LIBGSCSP_BUILD=$LIBGSCSP/build/
COMPILED=libgscsp.so
LIBGSCSP_ORIGINAL_BUILD=$LIBGSCSP/original_build/$COMPILED
SRC_MUTANTS=/opt/mutations/src-mutants/src
EXEC_DIR=$HOME/test_runs
FLAG=$1
COMMAND=(./waf build)

TCE=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/tce_sha512.sh

if [ ! -f "$LIBGSCSP_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json.template > $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json
    cat $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json
    
    cd $LIBGSCSP
    ./waf configure
    ./waf clean
    ./waf build
    mkdir -p original_build
    cp $LIBGSCSP_BUILD/$COMPILED original_build/
fi

source $TCE $LIBGSCSP $LIBGSCSP_SRC $LIBGSCSP_BUILD $LIBGSCSP_ORIGINAL_BUILD $FLAG $SRC_MUTANTS $COMPILED $EXEC_DIR $COMMAND

