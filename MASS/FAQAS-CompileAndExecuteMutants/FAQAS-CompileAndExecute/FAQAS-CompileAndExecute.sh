#!/bin/bash

sampling=$1
rate=$2
ts_prioritized_reduced=$3

all_filtered=$ALL_FILTERED
mutation_dir=$MUTATION_DIR
compilation_cmd="${COMPILATION_CMD[*]}"
additional_cmd="${ADDITIONAL_CMD[*]}"
additional_cmd_after="${ADDITIONAL_CMD_AFTER[*]}"
prioritized_file=$APP_RUN_DIR/PRIORITIZED/prioritized.txt
reduced_file=$APP_RUN_DIR/PRIORITIZED/reduced.txt

mut_exec_dir=$mutation_dir/test_runs

mkdir -p $mut_exec_dir

# clean list
>$mut_exec_dir/sampled_mutants

$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/sort_mutants.py --sampling "$sampling" --rate "$rate" --all_filtered "$all_filtered" --sampled_mutants "$mut_exec_dir/sampled_mutants"

if [ "$ts_prioritized_reduced" == "true" ];then
    $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/execute_mutants.py --sampling "$sampling" --mut_exec_dir "$mut_exec_dir" --compilation_cmd "$compilation_cmd" --additional_cmd "$additional_cmd" --additional_cmd_after "$additional_cmd_after" --prioritized "$prioritized_file" --reduced "$reduced_file" 
else
    $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/execute_mutants.py --sampling "$sampling" --mut_exec_dir "$mut_exec_dir" --compilation_cmd "$compilation_cmd" --additional_cmd "$additional_cmd" --additional_cmd_after "$additional_cmd_after" --prioritized "$prioritized_file"
fi

# generate list of live and killed mutants
$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/divide_killed_live.py --traces $mut_exec_dir/main.csv

awk -F';' '{print $1"|"$2}' $mut_exec_dir/traces_live.csv | sort | uniq > $mut_exec_dir/all_live
awk -F';' '{print $1"|"$2}' $mut_exec_dir/traces_killed.csv | sort | uniq > $mut_exec_dir/all_killed

 
