#!/bin/bash                                                                                                                        
 
HOME=/home/mlfs

MLFS=$HOME/mlfs
MLFS_SRC=$MLFS/libm
MLFS_TST=$MLFS/unit-reports
MLFS_COV=$MLFS_TST

COVERAGE_TRACES=/opt/mutations/runs_main/run_$1/test_runs/main.csv
MUTATION_TRACES=/opt/mutations/runs_main/all_traces_filtered.csv

EXEC_DIR=$HOME/test_runs

PYTHON=/usr/local/bin/python3.7

PROCESSING=/opt/srcirorfaqas/FAQASOptimizations/FAQASDetection/REDUNDANTS/processing_mlfs.py

MUTANT_LIST=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/NEW_SAMPLING/REDUCED_2/MLFS/all_filtered

TEST_PATH="/home/mlfs/unit-test-suite/"

$PYTHON -u $PROCESSING --coverages "$COVERAGE_TRACES" --mutants "$MUTANT_LIST" --exec_dir "$EXEC_DIR" --mut_traces "$MUTATION_TRACES" --test_path "$TEST_PATH"



