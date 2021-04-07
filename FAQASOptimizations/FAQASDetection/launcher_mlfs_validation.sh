#!/bin/bash                                                                                                                        
 
HOME=/home/mlfs

MLFS=$HOME/mlfs
MLFS_SRC=$MLFS/libm
MLFS_TST=$HOME/unit-reports
MLFS_COV=$MLFS_TST

SRC_MUTANTS=/opt/mutations/src-mutants/$1
MUTANTS_RUN=/opt/mutations/runs
MUTANTS_TRACES=/opt/mutations/runs_main

COV_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/mlfs/launcher_mlfs.sh

EXEC_DIR=$HOME/test_runs

PREFIX="libm"

RESULTS=$HOME/results

PYTHON=/usr/local/bin/python3.7
MUTANT_COVERAGE_FOLDER=$HOME/mutant_coverage


if [[ $2 == *"equivalents"* ]]; then
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/original_mutant_mlfs.sh
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/MLFS/all_live_validation
    DIST_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/distance.py
else
    MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/MLFS/all_filtered
    DETECTION=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/get_coverage_mlfs.sh
    DIST_SCRIPT=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/get_coverage.py
fi

source $DETECTION $MLFS $MLFS_SRC $MLFS_TST $MLFS_COV $SRC_MUTANTS $MUTANTS_RUN $COV_SCRIPT $DIST_SCRIPT $EXEC_DIR $MUTANT_LIST $RESULTS $PYTHON $MUTANT_COVERAGE_FOLDER $MUTANTS_TRACES $PREFIX
