#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

cd INSTALL_DIR_REPL
. ./mass_conf.sh

min=$1
max=$2

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/PostMutation_HPC.sh $min $max
