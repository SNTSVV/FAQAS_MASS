#!/bin/bash                                                                                                                        
 
HOME=/home/gsl

LIBUTIL=$HOME/libutil
LIBUTIL_SRC=$LIBUTIL/src
LIBUTIL_TST=$LIBUTIL/tst
LIBUTIL_COV=$LIBUTIL_TST

COVERAGE_TRACES=/opt/mutations/runs_main/run_$1/test_runs/main.csv
MUTATION_TRACES=/opt/mutations/runs_main/all_traces_filtered.csv

EXEC_DIR=$HOME/test_runs

PYTHON=/usr/bin/python3.7

PROCESSING=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/REDUNDANTS/processing.py

MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/LIBUTIL/all_filtered

TEST_PATH="/home/gsl/libutil/tst/"

$PYTHON -u $PROCESSING --coverages "$COVERAGE_TRACES" --mutants "$MUTANT_LIST" --exec_dir "$EXEC_DIR" --mut_traces "$MUTATION_TRACES" --test_path "$TEST_PATH"



