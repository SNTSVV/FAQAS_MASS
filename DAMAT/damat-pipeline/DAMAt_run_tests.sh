#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
# Modified by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

mutant_id=$1
tests_list=$2
DAMAT_FOLDER=$3
results_dir=$DAMAT_FOLDER/results

mutant_dir=$results_dir/run_"$mutant_id"
execution_log=$mutant_dir/"$mutant_id"_execution.out
coverage_file=$mutant_dir/"$mutant_id"_coverage.csv
results_file=$mutant_dir/main.csv

###############################################################################
#date in milliseconds
start_time=$(($(date +%s%N)/1000000))

mkdir $results_dir
echo "this is the results folder: $results_dir"
mkdir $mutant_dir
echo "this is the folder of this mutant $mutant_dir"
touch $execution_log
echo "this is the logfile of the tests' execution $execution_log"
touch $coverage_file
echo "this is the file with the results $results_file"
touch $results_file
echo "this is the coverage file: $coverage_file"
export FAQAS_COVERAGE_FILE=$coverage_file
export _FAQAS_SINGLETON_FM

###############################################################################

while IFS="," read -r p || [ -n "$p" ];do

    mutant_start_time=$(($(date +%s%N)/1000000))

    # obtaining test number to be executed
    read tst <<< $(awk -F',' '{print $1}' <<< "$p")

    # obtaining corresponding timeout for the test case
    TIMEOUT=$(echo "$p" | awk -F',' '{$2=($2*4)/1000; printf("%.0f\n", $2);}')

    echo "*********************************************************"
    echo "*********************************************************"
    echo "Running mutant $mutant_id against test case "$tst
    echo "*********************************************************"
    echo "*********************************************************"
    echo -n "${mutant_id};COMPILED;${tst};" >> $results_file


###############################################################################
# here the user shall call the execution of the current test case,
# we provided a simple example

  timeout $TIMEOUT bash execute_test_case.sh $tst

  EXEC_RET_CODE=$?

###############################################################################
#the exec return code should be 0 if the test case passes, 1 if the test case fails and 124 in case of a timeout


  mutant_end_time=$(($(date +%s%N)/1000000))
  mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"



  if [ $EXEC_RET_CODE -ge 124 ]; then
     echo "Test return code: [$EXEC_RET_CODE]"
     echo "Mutant timeout by $tst"
     echo -ne "TIMEOUT;KILLED_${EXEC_RET_CODE};${mutant_elapsed}\n" >> $results_file

  else
      if [ $EXEC_RET_CODE -eq 0 ]; then
        echo "Test return code: [$EXEC_RET_CODE]"
        echo -ne "PASSED;LIVE;${mutant_elapsed}\n" >> $results_file

      else
        echo "Test return code: [$EXEC_RET_CODE]"
        echo "Mutant killed by $tst"
        echo -ne "FAILED;KILLED;${mutant_elapsed}\n" >> $results_file
      fi
  fi


###############################################################################

	#create a different coverage file for every test

	NEW_COVERAGE_FILE=$mutant_dir/coverage_"$tst".csv
	cp $coverage_file $NEW_COVERAGE_FILE
	>$FAQAS_COVERAGE_FILE

done < $tests_list

rm -rf ~/Obsw/Test/System/testresults/*

end_time=$(($(date +%s%N)/1000000))
elapsed="$(($end_time-$start_time))"

echo "elapsed time $elapsed [ms]"
