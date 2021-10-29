#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

mutant_id_nr=$1
reduced_execution=$2

mutant_id=$(sed -n "${mutant_id_nr}p" $APP_RUN_DIR/MUTATION/test_runs/sampled_mutants)

export MUTANTS_DIR=$APP_RUN_DIR/hpc-src-mutants/m${mutant_id_nr}

sampling=$SAMPLING
ts_prioritized_reduced=$PRIORITIZED

mutation_dir=$APP_RUN_DIR/HPC_MUTATION/runs/run_${mutant_id_nr}

compilation_cmd="${COMPILATION_CMD[*]}"
additional_cmd="${ADDITIONAL_CMD[*]}"
additional_cmd_after="${ADDITIONAL_CMD_AFTER[*]}"
prioritized_file=$APP_RUN_DIR/PRIORITIZED/prioritized.txt
reduced_file=$APP_RUN_DIR/PRIORITIZED/reduced.txt
timeout_file=$APP_RUN_DIR/COV_FILES/test_suite_order.txt

mut_exec_dir=$mutation_dir/test_runs

if [ "$ts_prioritized_reduced" == "true" ] && [ "$sampling" == "stratified" ] || [ "$sampling" == "uniform" ];then
    echo "configuration not available, disabling prioritized reduced test suite"
    ts_prioritized_reduced="false"
fi

$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/execute_mutants_hpc.py --mutant_id "$mutant_id" --sampling "$sampling" --mut_exec_dir "$mut_exec_dir" --compilation_cmd "$compilation_cmd" --additional_cmd "$additional_cmd" --additional_cmd_after "$additional_cmd_after" --prioritized "$prioritized_file" --reduced "$reduced_file" --timeout "$timeout_file" --reduced_execution "$reduced_execution"

