#!/usr/bin/env bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#


DAMAT_FOLDER=$1
test_list=$2

RESULTS_FOLDER=$DAMAT_FOLDER/results
DATA_ANALYSIS_FOLDER=$DAMAT_FOLDER/data_analysis
mutants_table=$DAMAT_FOLDER/FAQAS_mutants_table.csv
readable_results=$RESULTS_FOLDER/readable_data.csv
mutator=$DAMAT_FOLDER/FAQAS_dataDrivenMutator.h
raw_data=$RESULTS_FOLDER/raw_data.csv
raw_data_sorted=$RESULTS_FOLDER/raw_data_sorted.csv
operator_coverage=$RESULTS_FOLDER/operator_coverage.csv
operator_coverage_sorted=$RESULTS_FOLDER/operator_coverage_sorted.csv
readable_operator_coverage=$RESULTS_FOLDER/readable_operator_coverage.csv
FM_coverage=$RESULTS_FOLDER/FM_coverage.csv
readable_FM_coverage=$RESULTS_FOLDER/readable_FM_coverage.csv
final_table=$RESULTS_FOLDER/final_mutants_table.csv

pushd $RESULTS_FOLDER

touch $raw_data

touch $FM_coverage

pushd $RESULTS_FOLDER

for f in *; do #2
  if [ -d "$f" ]; then #if1
    pushd $f
    cat main.csv >> $raw_data
    popd

    pushd $RESULTS_FOLDER
    if [ "$f" == "run_-2" ]; then #if2
      while IFS="," read -r p || [ -n "$p" ];do #3

        read tstcase <<< $(awk -F',' '{print $1}' <<< "$p")
        rm -f $RESULTS_FOLDER/$f/readable_coverage_"$tstcase".csv
        python $DATA_ANALYSIS_FOLDER/get_coverage.py "$tstcase" "$mutator" "$RESULTS_FOLDER/$f"
        cat $RESULTS_FOLDER/$f/readable_coverage_"$tstcase".csv >> $FM_coverage

      done < $test_list #3

      cat $FM_coverage | sort -n | uniq >> $readable_FM_coverage


    else #if2
      while IFS="," read -r p || [ -n "$p" ];do #4

        read tstcase <<< $(awk -F',' '{print $1}' <<< "$p")
        rm -f $RESULTS_FOLDER/$f/readable_coverage_"$tstcase".csv
        python $DATA_ANALYSIS_FOLDER/get_operator_coverage.py "$tstcase" "$RESULTS_FOLDER/$f"
        cat $RESULTS_FOLDER/$f/readable_coverage_$tstcase.csv >> $operator_coverage

      done < $test_list

      cat $operator_coverage | sort -n | uniq >> $readable_operator_coverage

    fi #if 2
    popd
  fi
done #2
popd


sort -n $raw_data > $raw_data_sorted

sort -n $readable_operator_coverage > $operator_coverage_sorted

python3 $DATA_ANALYSIS_FOLDER/beautify_results.py $raw_data_sorted "$mutants_table" $readable_results

python3 $DATA_ANALYSIS_FOLDER/get_stats.py $readable_results $readable_operator_coverage $readable_FM_coverage $mutants_table $RESULTS_FOLDER

python3 $DATA_ANALYSIS_FOLDER/get_final_results.py $final_table $RESULTS_FOLDER
