#!/bin/bash                                                                                                                        
 
HOME=/home/svf

ESAIL=$HOME/Obsw/Source
ESAIL_SRC=$ESAIL
ESAIL_TST=$HOME/mutant_coverage

ESAIL_COV=$ESAIL_TST

SRC_MUTANTS=/opt/mutations/src-mutants/$1
MUTANTS_RUN=/opt/mutations/runs
MUTANTS_TRACES=/opt/mutations/runs_main

COV_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/esail/launcher_esail.sh

EXEC_DIR=$HOME/test_runs

PREFIX=""

RESULTS=/opt/results

PYTHON=/usr/bin/python3.6
MUTANT_COVERAGE_FOLDER=$HOME/mutant_coverage
mkdir -p $MUTANT_COVERAGE_FOLDER

if [[ $2 == *"equivalents"* ]]; then
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/original_mutant_esail.sh
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/NEW_SAMPLING/REDUCED_2/ESAIL/all_live
    DIST_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/distance.py
else
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/get_coverage_esail.sh
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/NEW_SAMPLING/REDUCED_2/ESAIL/all_filtered
    DIST_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/get_coverage.py
fi

source $DETECTION $ESAIL $ESAIL_SRC $ESAIL_TST $ESAIL_COV $SRC_MUTANTS $MUTANTS_RUN $COV_SCRIPT $DIST_SCRIPT $EXEC_DIR $MUTANT_LIST $RESULTS $PYTHON $MUTANT_COVERAGE_FOLDER $MUTANTS_TRACES $PREFIX
