#!/bin/bash                                                                                                                        
HOME=/home/svf

EXEC_DIR=$HOME/test_runs

PYTHON=/usr/bin/python3.6

PROCESSING=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/PAIR-WISE/generate_data.py


TEST_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/test_cases
MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/all_live
COVERAGE_TRACES=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/mutants_coverage.csv
COVERAGE_ORIGINAL=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/original_coverage.csv


$PYTHON -u $PROCESSING --original_coverages "$COVERAGE_ORIGINAL" --coverages "$COVERAGE_TRACES" --tests "$TEST_LIST" --mutants "$MUTANT_LIST" --exec_dir "$EXEC_DIR"



