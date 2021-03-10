#!/bin/bash

min=$1
max=$2

mut_exec_dir=$MUTATION_DIR/test_runs
main=$mut_exec_dir/main.csv
results=$mut_exec_dir/results.csv

touch $main $results

if [ "$PRIORITIZED" == "true" ];then
    calibration=$mut_exec_dir/results_calibration.csv
    touch calibration
fi

for mutant_result in $(seq $min $max);do
    WD=$APP_RUN_DIR/HPC_MUTATION/runs/run_${mutant_result}/test_runs
    echo $WD

    mutant_data=$(find $WD -mindepth 1 -type d)
    rsync -avP --no-p --no-g --remove-source-files $mutant_data $mut_exec_dir
    cat $WD/main.csv >> $main
    cat $WD/results.csv >> $results
    
    if [ "$PRIORITIZED" == "true" ];then
        cat $WD/results_calibration.csv >> $calibration
    fi
done

if [ "$PRIORITIZED" == "true" ];then
    RET=$($MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/continue_mutation_hpc.py --sampling "$SAMPLING" --mut_exec_dir "$mut_exec_dir" --reduced "$APP_RUN_DIR/PRIORITIZED/reduced.txt")
else
    RET=$($MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/continue_mutation_hpc.py --sampling "$SAMPLING" --mut_exec_dir "$mut_exec_dir")
fi

case $RET in
    0)
        echo "MASS: No more mutant executions are needed"

        # generate list of live and killed mutants
        $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/divide_killed_live.py --traces $mut_exec_dir/main.csv

        awk -F';' '{print $1"|"$2}' $mut_exec_dir/traces_live.csv | sort | uniq > $mut_exec_dir/all_live
        awk -F';' '{print $1"|"$2}' $mut_exec_dir/traces_killed.csv | sort | uniq > $mut_exec_dir/all_killed
        ;;
    1)
        echo "MASS: More mutant executions with full test suite are necessary"
        ;;
    2)
        echo "MASS: More mutant executions with reduced test suite are necessary"
        ;;
esac

