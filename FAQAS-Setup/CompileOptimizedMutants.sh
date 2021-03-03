#!/bin/bash 

cd INSTALL_DIR_REPL
. ./mass_conf.sh

if [ "$HPC" == "true" ];then
    HPC_FLAG=$1
    source $MASS/FAQAS-CompileOptimizedMutants/FAQAS-CompileOptimizedMutants.sh "${FLAGS[$HPC_FLAG]}" $HPC_FLAG
else
    # run sequentially
    run=0
    for FLAG in "${FLAGS[@]}";do
        source $MASS/FAQAS-CompileOptimizedMutants/FAQAS-CompileOptimizedMutants.sh $FLAG $run
        run=$((run+1))
    done
fi
