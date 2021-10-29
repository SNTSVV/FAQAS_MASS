#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

cd INSTALL_DIR_REPL
. ./mass_conf.sh

mutant_folder=$1
reduced=$2

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/FAQAS-CompileAndExecute.sh $mutant_folder $reduced
