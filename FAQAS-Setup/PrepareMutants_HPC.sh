#!/bin/bash

cd INSTALL_DIR_REPL
. ./mass_conf.sh

SAMPLING=$1
RATE=$2

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/PrepareMutantExecution_HPC.sh $SAMPLING $RATE
