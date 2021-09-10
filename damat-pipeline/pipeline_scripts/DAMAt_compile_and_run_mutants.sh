#!/bin/sh

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#



DAMAt_FOLDER=$1
PIPELINE_FOLDER=$DAMAt_FOLDER/pipeline_scripts
RESULTS_FOLDER=$DAMAt_FOLDER/results
mkdir -p $RESULTS_FOLDER
LOGS_FOLDER=$RESULTS_FOLDER/logs
mkdir -p $LOGS_FOLDER

TESTS_FOLDER=$DAMAt_FOLDER/testlists

#mutant=0

pushd $DAMAt_FOLDER
   max_mutant=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
   echo "this is the max mutation operator: $max_mutant"
popd

#while [ "$mutant" -le $max_mutant ]; do
for mutant in 0 18; do

   mutant_log=$LOGS_FOLDER/mutant_"$mutant"_log.out

   touch $mutant_log

   echo "*****************************************************************" 2>&1 | tee -a $mutant_log

   echo "*****************************************************************" 2>&1 | tee -a $mutant_log

   echo "executing mutant $mutant"  2>&1 | tee -a $mutant_log

   echo "*****************************************************************"  2>&1 | tee -a $mutant_log

   echo "*****************************************************************"  2>&1 | tee -a $mutant_log


  # COMPILE MUTANT

  bash $DAMAt_FOLDER/DAMAt_compile.sh "$mutant"  2>&1 | tee -a $mutant_log

  # RUN MUTANT

  tests_list=$TESTS_FOLDER/tests_"$mutant"

  bash $DAMAt_FOLDER/DAMAt_run_tests.sh "$mutant" $tests_list $DAMAt_FOLDER  2>&1 | tee -a $mutant_log

  mutant=$((mutant+1))

done
