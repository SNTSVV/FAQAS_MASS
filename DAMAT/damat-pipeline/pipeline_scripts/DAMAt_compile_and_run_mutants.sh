#!/bin/sh

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#


DAMAt_FOLDER=$1

. $DAMAt_FOLDER/DAMAt_configure.sh

echo "PIPELINE_FOLDER=$PIPELINE_FOLDER"
echo "RESULTS_FOLDER=$RESULTS_FOLDER"
echo "LOGS_FOLDER=$LOGS_FOLDER"
echo "TESTS_FOLDER=$TESTS_FOLDER"
echo "DATA_ANALYSIS_FOLDER=$DATA_ANALYSIS_FOLDER"
echo "mutator=$mutator"
echo "mutants_table=$mutants_table"

mkdir -p $RESULTS_FOLDER
mkdir -p $LOGS_FOLDER

mutant=0

pushd $DAMAt_FOLDER
   max_mutant=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
   echo "this is the max mutation operator: $max_mutant"
popd

while [ "$mutant" -le $max_mutant ]; do
#for mutant in 0 18; do

   mutant_log=$LOGS_FOLDER/mutant_"$mutant"_log.out

   touch $mutant_log

   echo "*****************************************************************" 2>&1 | tee -a $mutant_log

   echo "*****************************************************************" 2>&1 | tee -a $mutant_log

   echo "executing mutant $mutant"  2>&1 | tee -a $mutant_log

   echo "*****************************************************************"  2>&1 | tee -a $mutant_log

   echo "*****************************************************************"  2>&1 | tee -a $mutant_log


  # COMPILE MUTANT

  bash $DAMAt_FOLDER/DAMAt_compile.sh "$mutant" $singleton  2>&1 | tee -a $mutant_log

  # RUN MUTANT

  tests_list=$TESTS_FOLDER/tests_"$mutant"

  bash $DAMAt_FOLDER/DAMAt_run_tests.sh "$mutant" $tests_list $DAMAt_FOLDER  2>&1 | tee -a $mutant_log

  mutant=$((mutant+1))

done
