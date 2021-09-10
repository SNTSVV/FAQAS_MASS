#!/bin/sh
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

tests_list=$1
DAMAt_FOLDER=$2
mutator=$DAMAt_FOLDER/FAQAS_dataDrivenMutator.h
mutants_table=$DAMAt_FOLDER/FAQAS_mutants_table.csv

PIPELINE_FOLDER=$DAMAt_FOLDER/pipeline_scripts
DATA_ANALYSIS_FOLDER=$DAMAt_FOLDER/data_analysis
RESULTS_FOLDER=$DAMAt_FOLDER/results
mkdir -p $RESULTS_FOLDER
FM_coverage=$RESULTS_FOLDER/FM_coverage.csv
readable_FM_coverage=$RESULTS_FOLDER/readable_FM_coverage.csv

LOGS_FOLDER=$RESULTS_FOLDER/logs
mkdir -p $LOGS_FOLDER

coverage_log=$LOGS_FOLDER/coverage_log.out
touch coverage_log

TESTS_FOLDER=$DAMAt_FOLDER/testlists
mkdir -p $TESTS_FOLDER

#compile the special mutant
bash $DAMAt_FOLDER/DAMAt_compile.sh "-2" 2>&1 | tee -a $coverage_log
#execute the test suite
bash $DAMAt_FOLDER/DAMAt_run_tests.sh "-2" $tests_list $DAMAt_FOLDER 2>&1 | tee -a $coverage_log

 pushd $RESULTS_FOLDER
   while IFS="," read -r p || [ -n "$p" ];do #3

     read tstcase <<< $(awk -F',' '{print $1}' <<< "$p")
     rm -f $RESULTS_FOLDER/run_-2/readable_coverage_"$tstcase".csv
     python $DATA_ANALYSIS_FOLDER/get_coverage.py "$tstcase" "$mutator" "$RESULTS_FOLDER/run_-2"
     cat $RESULTS_FOLDER/run_-2/readable_coverage_"$tstcase".csv >> $FM_coverage

   done < $tests_list #3

   cat $FM_coverage | sort -n | uniq >> $readable_FM_coverage

  popd

 python3 $PIPELINE_FOLDER/get_mutant_test_list.py $readable_FM_coverage $tests_list $mutants_table $TESTS_FOLDER
