#!/bin/bash

sampling=$1
rate=$2

all_filtered=$APP_RUN_DIR/COMPILED/all_filtered
mutation_dir=$MUTATION_DIR

mut_exec_dir=$mutation_dir/test_runs
mkdir -p $mut_exec_dir

hpc_mutants_dir=$APP_RUN_DIR/hpc-src-mutants
mkdir -p $hpc_mutants_dir

# clean list of sampled mutants
>$mut_exec_dir/sampled_mutants

###

$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/sort_mutants.py --sampling "$sampling" --rate "$rate" --all_filtered "$all_filtered" --sampled_mutants "$mut_exec_dir/sampled_mutants"

####

cd $MUTANTS_DIR

n=1
while IFS="" read -r p || [ -n "$p" ];do
    echo "$n"
    
    folder="${hpc_mutants_dir}/m${n}"
    mkdir -p $folder

    mutant_name=$(echo $p | awk -F'|' '{print $1}')
    mutant_location=$(echo $p | awk -F'|' '{print $2}' | sed "s:\.c::")
    mutant_full_path=$(find . -wholename "*$mutant_location*$mutant_name.c")

    cp "$mutant_full_path" --parents $folder
    n=$((n+1))

done < $mut_exec_dir/sampled_mutants

