#!/bin/bash                                                                                                                        
 
HOME=/home/csp

LIBPARAM=$HOME/libparam
LIBPARAM_SRC=$LIBPARAM/src
LIBPARAM_TST=$LIBPARAM/tst
LIBPARAM_COV=$LIBPARAM_TST

SRC_MUTANTS=/opt/mutations/src-mutants/$1
MUTANTS_RUN=/opt/mutations/runs

COV_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/libparam/launcher_param.sh
DIST_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/distance.py

EXEC_DIR=$HOME/test_runs

PREFIX="src"

RESULTS=$HOME/results

PYTHON=/usr/bin/python3.7
MUTANT_COVERAGE_FOLDER=$HOME/mutant_coverage


if [[ $2 == *"equivalents"* ]]; then
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/original_mutant.sh
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/PARAM_GSL/all_live
else
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/mutant_mutant.sh
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/PARAM_GSL/all_filtered
fi

source $DETECTION $LIBPARAM $LIBPARAM_SRC $LIBPARAM_TST $LIBPARAM_COV $SRC_MUTANTS $MUTANTS_RUN $COV_SCRIPT $DIST_SCRIPT $EXEC_DIR $MUTANT_LIST $RESULTS $PYTHON $MUTANT_COVERAGE_FOLDER $PREFIX
