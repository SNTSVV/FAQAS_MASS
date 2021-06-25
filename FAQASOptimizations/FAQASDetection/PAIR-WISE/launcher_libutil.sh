#!/bin/bash                                                                             

HOME=/home/gsl

COVERAGE_TRACES=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/LIBUTIL/mutants_coverage.csv
COVERAGE_ORIGINAL=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/LIBUTIL/original_coverage.csv

EXEC_DIR=$HOME/test_runs

PYTHON=/usr/bin/python3.7

PROCESSING=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/PAIR-WISE/processing.py

MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/LIBUTIL/all_live

TEST_PATH="/home/gsl/libutil/tst/"

$PYTHON -u $PROCESSING --original_coverages "$COVERAGE_ORIGINAL" --coverages "$COVERAGE_TRACES" --mutants "$MUTANT_LIST" --exec_dir "$EXEC_DIR" --mut_traces "$MUTATION_TRACES" --test_path "$TEST_PATH"



