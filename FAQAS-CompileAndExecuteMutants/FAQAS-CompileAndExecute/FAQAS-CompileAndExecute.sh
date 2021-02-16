#!/bin/bash

sampling=$1
rate=$2
ts_prioritized=$3
all_filtered=$ALL_FILTERED
mutation_dir=$MUTATION_DIR
compilation_cmd="${COMPILATION_CMD[*]}"
additional_cmd="${ADDITIONAL_CMD[*]}"
additional_cmd_after="${ADDITIONAL_CMD_AFTER[*]}"
prioritized_file=$APP_RUN_DIR/PRIORITIZED/prioritized.txt
full_tst_order_file=$APP_RUN_DIR/COV_FILES/test_suite_order.txt

mut_exec_dir=$mutation_dir/test_runs

mkdir -p $mut_exec_dir

# clean list
>$mut_exec_dir/sampled_mutants

$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/sort_mutants.py --sampling "$sampling" --rate "$rate" --all_filtered "$all_filtered" --sampled_mutants "$mut_exec_dir/sampled_mutants"

if [ "$ts_prioritized" == "true" ];then
    $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/execute_mutants.py --sampling "$sampling" --mut_exec_dir "$mut_exec_dir" --compilation_cmd "$compilation_cmd" --additional_cmd "$additional_cmd" --additional_cmd_after "$additional_cmd_after" --prioritized "$prioritized_file" --full_tst_order "$full_tst_order_file"
else
    $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/execute_mutants.py --sampling "$sampling" --mut_exec_dir "$mut_exec_dir" --compilation_cmd "$compilation_cmd" --additional_cmd "$additional_cmd" --additional_cmd_after "$additional_cmd_after" --full_tst_order "$full_tst_order_file"
fi
