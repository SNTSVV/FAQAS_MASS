#!/bin/bash

HOME=/home/csp 
LIBPARAM=$HOME/libparam
LIBPARAM_SRC=$LIBPARAM/src
LIBPARAM_TST=$LIBPARAM/tst
LIBPARAM_COV=$LIBPARAM_TST
SRC_MUTANTS=/opt/mutations/src-mutants/$1
BUILD_COMMAND=(./waf build --notests)
EXECUTION_COMMAND=(./waf --alltests -v)
FILTER_TST="include|example|bindings"
EXEC_DIR=$HOME/test_runs
TIMEOUT=250

MUTATION=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutation/mutation.sh

cd $LIBPARAM
find . -name '*.gcda' -delete

source $MUTATION $LIBPARAM $LIBPARAM_SRC $LIBPARAM_TST $LIBPARAM_COV $SRC_MUTANTS $BUILD_COMMAND $EXECUTION_COMMAND $FILTER_TST $EXEC_DIR $TIMEOUT
