#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

# This file should be prepared by the engineer!
cd INSTALL_DIR_REPL
. ./mass_conf.sh

# 1. Compile SUT
## example

# cd $PROJ

# # generate compile_commands.json and delete build
# bear make all && rm -rf build* && sed -i 's: libm: /home/mlfs/mlfs/libm:' compile_commands.json && mv compile_commands.json $MUTANTS_DIR
# eval "${COMPILATION_CMD[@]}"

# 2. Prepare test scripts
# example

# cd $HOME/blts/BLTSConfig
# make clean install INSTALL_PATH="$HOME/blts_install"

# Preparing MLFS workspace (e.g., where test cases data is stored)
# cd $HOME/blts_workspace
# $HOME/blts_install/bin/blts_app --init

# 3. Execute test cases
# Note: execution time for each test case should be measured and passed as argument to FAQAS-CollectCodeCoverage.sh

# example
#for tst in $(find $HOME/unit-test-suite -name '*.xml');do
#    cd $HOME/blts_workspace

#    tst_filename_wo_xml=$(basename -- $tst .xml)
    
#    start=$(date +%s)
#    $HOME/blts_install/bin/blts_app -gcrx $tst_filename_wo_xml -b coverage --nocsv -s $tst
#    end=$(date +%s)    

#    # call to FAQAS-CollectCodeCoverage.sh
#    # parameter should be test case name and the execution time
#    source $MASS/FAQAS-GenerateCodeCoverageMatrixes/FAQAS-CollectCodeCoverage.sh $tst_filename_wo_xml "$(($end-$start))" 
#done

