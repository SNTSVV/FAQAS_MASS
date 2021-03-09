#!/bin/bash

cd INSTALL_DIR_REPL
. ./mass_conf.sh

min=$1
max=$2

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/PostMutation_HPC.sh $min $max
