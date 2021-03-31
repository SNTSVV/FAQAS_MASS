#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

# backup useful files
mkdir -p $APP_RUN_DIR/RESULTS

RESULT_FILE=$APP_RUN_DIR/RESULTS/MASS_RESULTS
touch $RESULT_FILE

cp $APP_RUN_DIR/DETECTION/test_runs/useful_list_* $APP_RUN_DIR/RESULTS
cp $APP_RUN_DIR/DETECTION/test_runs/equivalents_distance_zero $APP_RUN_DIR/RESULTS
cp $MUTATION_DIR/test_runs/all_killed $APP_RUN_DIR/RESULTS
cp $MUTATION_DIR/test_runs/filtered_live $APP_RUN_DIR/RESULTS

all=$(cat $APP_RUN_DIR/COMPILED/all | wc -l)
all_filtered=$(cat $APP_RUN_DIR/COMPILED/all_filtered | wc -l)
killed=$(cat $MUTATION_DIR/test_runs/all_killed | wc -l)
live=$(cat $MUTATION_DIR/test_runs/all_live | wc -l)
live_filtered=$(cat $MUTATION_DIR/test_runs/filtered_live | wc -l)
zero=$(cat $APP_RUN_DIR/RESULTS/equivalents_distance_zero | wc -l)

MASS_ms=$(echo "scale=2; 100*${killed}/(${killed}+${live_filtered})" | bc -l)

echo "##### MASS Output #####" 2>&1 | tee -a $RESULT_FILE
echo "## Total mutants generated: $all" 2>&1 | tee -a $RESULT_FILE
echo "## Total mutants filtered by TCE: $((all-all_filtered))" 2>&1 | tee -a $RESULT_FILE
echo "## Sampling type: $SAMPLING" 2>&1 | tee -a $RESULT_FILE
echo "## Total mutants analyzed: $((killed+live))"  2>&1 | tee -a $RESULT_FILE
echo "## Total killed mutants: $killed" 2>&1 | tee -a $RESULT_FILE
echo "## Total live mutants: $live"  2>&1 | tee -a $RESULT_FILE
echo "## Total likely equivalent mutants: $zero" 2>&1 | tee -a $RESULT_FILE
echo "## MASS mutation score (%): $MASS_ms"  2>&1 | tee -a $RESULT_FILE

echo "## List A of useful undetected mutants: $APP_RUN_DIR/DETECTION/test_runs/useful_list_a"  2>&1 | tee -a $RESULT_FILE
echo "## List B of useful undetected mutants: $APP_RUN_DIR/DETECTION/test_runs/useful_list_b"  2>&1 | tee -a $RESULT_FILE

source $MASS/FAQAS-MutationScore/statement_coverage.sh $APP_RUN_DIR/COV_FILES/coverage.txt.overall

$MASS/FAQAS-MutationScore/report_coverage.py --coverage_file "$APP_RUN_DIR/RESULTS/statement_coverage.txt" --result_file "$APP_RUN_DIR/RESULTS/report_coverage.csv"

lines_covered=$(awk -F';' '{print $1}' $APP_RUN_DIR/RESULTS/report_coverage.csv)
statement_coverage=$(awk -F';' '{print $2}' $APP_RUN_DIR/RESULTS/report_coverage.csv)
min_lines=$(awk -F';' '{print $3}' $APP_RUN_DIR/RESULTS/report_coverage.csv)
max_lines=$(awk -F';' '{print $4}' $APP_RUN_DIR/RESULTS/report_coverage.csv)

echo "## Number of statements covered: $lines_covered" 2>&1 | tee -a $RESULT_FILE
echo "## Statement coverage (%): $statement_coverage" 2>&1 | tee -a $RESULT_FILE
echo "## Minimum lines covered per source file: $min_lines" 2>&1 | tee -a $RESULT_FILE
echo "## Maximum lines covered per source file: $max_lines" 2>&1 | tee -a $RESULT_FILE

