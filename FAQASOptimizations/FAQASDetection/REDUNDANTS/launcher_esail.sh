#!/bin/bash                                                                                                                        
 
HOME=/home/svf

ESAIL=$HOME/Obsw/Source
ESAIL_SRC=$ESAIL
ESAIL_TST=$ESAIL/mutant_coverage

ESAIL_COV=$ESAIL_TST

COVERAGE_TRACES=/opt/mutations/runs_main/run_$1/test_runs/main.csv
MUTATION_TRACES=/opt/mutations/runs_main/all_traces_filtered.csv

EXEC_DIR=$HOME/test_runs

PYTHON=/usr/bin/python3.6

PROCESSING=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/REDUNDANTS/processing_esail.py

MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/all_filtered

TEST_PATH=""

$PYTHON -u $PROCESSING --coverages "$COVERAGE_TRACES" --mutants "$MUTANT_LIST" --exec_dir "$EXEC_DIR" --mut_traces "$MUTATION_TRACES" --test_path "$TEST_PATH"



