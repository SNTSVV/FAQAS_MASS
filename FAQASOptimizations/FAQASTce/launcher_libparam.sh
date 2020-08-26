#!/bin/bash

HOME=/home/csp
LIBPARAM=$HOME/libparam
LIBPARAM_SRC=$LIBPARAM/src
LIBPARAM_BUILD=$LIBPARAM/build
COMPILED=libgsparam.so
LIBPARAM_ORIGINAL_BUILD=$LIBPARAM/original_build/$COMPILED
SRC_MUTANTS=/opt/mutations/src-mutants
EXEC_DIR=$HOME/test_runs
FLAG=$1
COMMAND=(./waf build)

TCE=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/tce_sha512.sh

if [ ! -f "$LIBPARAM_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json.template > $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json
    cat $HOME/libparam/tools/buildtools/gs/buildtools/compiler_settings.json
    
    cd $LIBPARAM
    ./waf configure
    ./waf clean
    ./waf build
    mkdir -p original_build
    cp $LIBPARAM_BUILD/$COMPILED original_build/
fi

source $TCE $LIBPARAM $LIBPARAM_SRC $LIBPARAM_BUILD $LIBPARAM_ORIGINAL_BUILD $FLAG $SRC_MUTANTS $COMPILED $EXEC_DIR $COMMAND

