#!/bin/bash

sampling=$SAMPLING
rate=$RATE
ts_prioritized_reduced=$PRIORITIZED

all_filtered=$APP_RUN_DIR/COMPILED/all_filtered
mutation_dir=$MUTATION_DIR
compilation_cmd="${COMPILATION_CMD[*]}"
additional_cmd="${ADDITIONAL_CMD[*]}"
additional_cmd_after="${ADDITIONAL_CMD_AFTER[*]}"
prioritized_file=$APP_RUN_DIR/PRIORITIZED/prioritized.txt
reduced_file=$APP_RUN_DIR/PRIORITIZED/reduced.txt
timeout_file=$APP_RUN_DIR/COV_FILES/test_suite_order.txt

mut_exec_dir=$mutation_dir/test_runs

if [ -d $mut_exec_dir ];then
    echo "Backing up previous mutation folder"
    nr_tests=$(ls $mutation_dir | wc -l)
    mv $mut_exec_dir $mutation_dir/test_runs_$nr_tests
fi

mkdir -p $mut_exec_dir

# clean list
>$mut_exec_dir/sampled_mutants

$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/sort_mutants.py --sampling "$sampling" --rate "$rate" --all_filtered "$all_filtered" --sampled_mutants "$mut_exec_dir/sampled_mutants"

if [ "$ts_prioritized_reduced" == "true" ] && [ "$sampling" == "stratified" ] || [ "$sampling" == "uniform" ];then
    echo "configuration not available, disabling prioritized reduced test suite"
    ts_prioritized_reduced="false"
fi

if [ "$ts_prioritized_reduced" == "true" ];then
    $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/execute_mutants.py --sampling "$sampling" --mut_exec_dir "$mut_exec_dir" --compilation_cmd "$compilation_cmd" --additional_cmd "$additional_cmd" --additional_cmd_after "$additional_cmd_after" --prioritized "$prioritized_file" --reduced "$reduced_file" --timeout "$timeout_file"
else
    $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/execute_mutants.py --sampling "$sampling" --mut_exec_dir "$mut_exec_dir" --compilation_cmd "$compilation_cmd" --additional_cmd "$additional_cmd" --additional_cmd_after "$additional_cmd_after" --prioritized "$prioritized_file" --timeout "$timeout_file"
fi

# generate list of live and killed mutants
$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/divide_killed_live.py --traces $mut_exec_dir/main.csv

awk -F';' '{print $1"|"$2}' $mut_exec_dir/traces_live.csv | sort | uniq > $mut_exec_dir/all_live
awk -F';' '{print $1"|"$2}' $mut_exec_dir/traces_killed.csv | sort | uniq > $mut_exec_dir/all_killed

