#!/bin/bash

HOME=/home/csp 
LIBPARAM=$HOME/libparam
LIBPARAM_SRC=$LIBPARAM/src
LIBPARAM_TST=$LIBPARAM/tst
LIBPARAM_COV=$LIBPARAM_TST
SRC_MUTANTS=/opt/mutations/src-mutants/m$1
BUILD_COMMAND=(./waf build --notests)
EXECUTION_COMMAND=(./waf --alltests -v)
FILTER_TST="include|example|bindings"
EXEC_DIR=$HOME/test_runs
TIMEOUT=250

MUTATION=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutation/mutation.sh

shopt -s extglob
cd $LIBPARAM

./tools/buildtools/gsbuildtools_bootstrap.py

for tst in $LIBPARAM_TST/!($FILTER_TST)/;do
    echo $tst
    cd $tst
    ./waf configure
    cd ..
done

source $MUTATION $LIBPARAM $LIBPARAM_SRC $LIBPARAM_TST $LIBPARAM_COV $SRC_MUTANTS $BUILD_COMMAND $EXECUTION_COMMAND $FILTER_TST $EXEC_DIR $TIMEOUT
