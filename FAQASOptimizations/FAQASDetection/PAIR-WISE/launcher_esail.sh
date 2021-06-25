#!/bin/bash                                                                                                                        
HOME=/home/svf

EXEC_DIR=$HOME/test_runs

PYTHON=/usr/bin/python3.6

PROCESSING=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/PAIR-WISE/processing.py

MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/all_live
COVERAGE_TRACES=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/mutants_coverage.csv
COVERAGE_ORIGINAL=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/original_coverage.csv

COMBINATION=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/combinations/combination_$1

$PYTHON -u $PROCESSING --original_coverages "$COVERAGE_ORIGINAL" --coverages "$COVERAGE_TRACES" --mutants "$MUTANT_LIST" --exec_dir "$EXEC_DIR" --combination "$COMBINATION"



