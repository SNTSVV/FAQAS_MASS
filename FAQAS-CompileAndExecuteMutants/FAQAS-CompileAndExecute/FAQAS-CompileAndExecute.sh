#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

if [ "$HPC" == "false" ];then
    source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/CompileAndExecute.sh
else
    mutant_folder=$1
    reduced_execution=$2

    source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/CompileAndExecute_HPC.sh $mutant_folder $reduced_execution
fi

