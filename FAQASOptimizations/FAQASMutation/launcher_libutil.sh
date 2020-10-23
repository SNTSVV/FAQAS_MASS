#!/bin/bash

HOME=/home/gsl                                                                                                       
LIBUTIL=$HOME/libutil
LIBUTIL_SRC=$LIBUTIL/src
LIBUTIL_TST=$LIBUTIL/tst
LIBUTIL_COV=$LIBUTIL_TST
SRC_MUTANTS=/opt/mutations/src-mutants/$1
BUILD_COMMAND=(./waf build --notests)
EXECUTION_COMMAND=(./waf --alltests -v)
FILTER_TST="can_sniffer|rtc|stdio|util-app"
EXEC_DIR=$HOME/test_runs
TIMEOUT=250


MUTATION=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutation/mutation.sh

cd $LIBUTIL
find . -name '*.gc*' -delete

source $MUTATION $LIBUTIL $LIBUTIL_SRC $LIBUTIL_TST $LIBUTIL_COV $SRC_MUTANTS $BUILD_COMMAND $EXECUTION_COMMAND $FILTER_TST $EXEC_DIR $TIMEOUT
