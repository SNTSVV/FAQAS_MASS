#!/bin/bash

min=$1
max=$2

final_wd=$MUTATION_DIR/test_runs
main=$final_wd/main.csv
results=$final_wd/results.csv
calibration=$final_wd/results_calibration.csv

touch $main $results $calibration

for mutant_result in $(seq $min $max);do
    WD=$APP_RUN_DIR/HPC_MUTATION/runs/run_${mutant_result}/test_runs
    echo $WD

    mutant_data=$(find $WD -mindepth 1 -type d)
    rsync -avP --no-p --no-g --remove-source-files $mutant_data $final_wd
    cat $WD/main.csv >> $main
    cat $WD/results.csv >> $results
    cat $WD/results_calibration.csv >> $calibration
done
