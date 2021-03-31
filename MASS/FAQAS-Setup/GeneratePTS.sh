#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

# 1. FAQAS-GeneratePrioritizedTestSuite
cd INSTALL_DIR_REPL
. ./mass_conf.sh

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-GeneratePrioritizedTestSuite/FAQAS-GeneratePrioritizedTestSuite.sh

