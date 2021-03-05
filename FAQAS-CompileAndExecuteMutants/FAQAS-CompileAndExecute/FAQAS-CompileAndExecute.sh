#!/bin/bash

if [ "$HPC" == "false" ];then
    source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/CompileAndExecute.sh
else
    mutant_folder=$1
    reduced_execution=$2

    source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/CompileAndExecute_HPC.sh $mutant_folder $reduced_execution
fi

