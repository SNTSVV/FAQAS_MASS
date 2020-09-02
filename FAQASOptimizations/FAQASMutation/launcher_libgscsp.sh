#!/bin/bash

HOME=/home/csp
LIBGSCSP=$HOME/libparam/lib/libgscsp
LIBGSCSP_SRC=$LIBGSCSP
LIBGSCSP_TST=$LIBGSCSP/tst
LIBGSCSP_COV=$LIBGSCSP_TST
SRC_MUTANTS=/opt/mutations/src-mutants/$1
BUILD_COMMAND=(./waf build --notests)
EXECUTION_COMMAND=(./waf --alltests -v)
FILTER_TST="bindings|common"
EXEC_DIR=$HOME/test_runs
TIMEOUT=450

MUTATION=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutation/mutation.sh

cd $LIBGSCSP
find . -name '*.gcda' -delete

source $MUTATION $LIBGSCSP $LIBGSCSP_SRC $LIBGSCSP_TST $LIBGSCSP_COV $SRC_MUTANTS $BUILD_COMMAND $EXECUTION_COMMAND $FILTER_TST $EXEC_DIR $TIMEOUT
