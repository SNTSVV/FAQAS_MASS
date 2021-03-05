#!/bin/bash

cd INSTALL_DIR_REPL
. ./mass_conf.sh

mutant_folder=$1
reduced=$2

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/FAQAS-CompileAndExecute.sh $mutant_folder $reduced
