#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

cd INSTALL_DIR_REPL
. ./mass_conf.sh

SAMPLING=$1
RATE=$2

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/PrepareMutantExecution_HPC.sh $SAMPLING $RATE
