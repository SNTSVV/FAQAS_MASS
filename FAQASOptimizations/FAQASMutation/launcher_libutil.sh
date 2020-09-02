#!/bin/bash

HOME=/home/gsl                                                                                                       
LIBUTIL=$HOME/libutil
LIBUTIL_SRC=$LIBUTIL/src
LIBUTIL_TST=$LIBUTIL/tst
LIBUTIL_COV=$LIBUTIL_TST
SRC_MUTANTS=/opt/mutations/src-mutants
BUILD_COMMAND=(./waf build --notests)
EXECUTION_COMMAND=(./waf --alltests -v)
FILTER_TST="can_sniffer|rtc|stdio|util-app"
EXEC_DIR=$HOME/test_runs
TIMEOUT=250

MUT_INPUT=$1

MUTATION=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutation/mutation.sh

cd $LIBUTIL
find . -name '*.gcda' -delete

while IFS="" read -r mutant || [ -n "$mutant" ];do
	source $MUTATION $LIBUTIL $LIBUTIL_SRC $LIBUTIL_TST $LIBUTIL_COV $SRC_MUTANTS $BUILD_COMMAND $EXECUTION_COMMAND $FILTER_TST $EXEC_DIR $TIMEOUT $mutant
done < $MUT_INPUT
