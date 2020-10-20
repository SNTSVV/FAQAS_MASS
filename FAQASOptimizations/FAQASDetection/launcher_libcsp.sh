#!/bin/bash                                                                                                                        
 
HOME=/home/csp

LIBCSP=$HOME/libparam/lib/libgscsp
LIBCSP_SRC=$LIBCSP
LIBCSP_TST=$LIBCSP/tst
LIBCSP_COV=$LIBCSP_TST

SRC_MUTANTS=/opt/mutations/src-mutants/$1
MUTANTS_RUN=/opt/mutations/runs

COV_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/libgscsp/launcher_csp.sh
DIST_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/distance.py

EXEC_DIR=$HOME/test_runs

RESULTS=$HOME/results

PYTHON=/usr/bin/python3.7
MUTANT_COVERAGE_FOLDER=$HOME/mutant_coverage

PREFIX=

cd $HOME
shopt -s expand_aliases
source .bashrc
source .profile

if [[ $2 == *"equivalents"* ]]; then
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/original_mutant.sh
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/CSP_GSL/all_live
else
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/mutant_mutant.sh
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/CSP_GSL/all_filtered
fi

source $DETECTION $LIBCSP $LIBCSP_SRC $LIBCSP_TST $LIBCSP_COV $SRC_MUTANTS $MUTANTS_RUN $COV_SCRIPT $DIST_SCRIPT $EXEC_DIR $MUTANT_LIST $RESULTS $PYTHON $MUTANT_COVERAGE_FOLDER $PREFIX
