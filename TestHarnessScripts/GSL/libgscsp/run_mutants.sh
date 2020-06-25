#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

SRC_MUTANTS=/opt/mutations/mutants/$1

LIBGSCSP=/home/csp/libparam/lib/libgscsp

EXEC_NAME=test_run_`date +"%Y%m%d%H%M"`
EXEC_DIR=/home/csp/test_runs/$EXEC_NAME

LOGFILE=$EXEC_DIR/$EXEC_NAME.log
mkdir -p $EXEC_DIR

touch $LOGFILE

TIMEOUT=400

shopt -s extglob
trap "exit" INT

for i in `find $SRC_MUTANTS -name '*.c'`; do
    start_time="$(date -u +%s)"

    file_wo_opt=${i//$SRC_MUTANTS/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"
    file_wo_mut_end=.${file_wo_opt%%.*}.c

    filename="$(basename -- ${file_wo_mut_end%.*})"
    filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")

    mutant_path=$EXEC_DIR/$mutant_name

    mkdir -p $mutant_path

    MUTANT_LOGFILE=$mutant_path/logfile
    touch $MUTANT_LOGFILE

    echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
    echo "Mutant: "$i 2>&1 | tee -a $MUTANT_LOGFILE

    cd $LIBGSCSP

    # replacing mutant by original source
    echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
    cp $filename_orig $filename_orig.orig

    echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
    cp $i $filename_orig

    # make a copy of the mutant
    cp $i $mutant_path

    compilation_success=1
    mutant_killed=0
    mutant_timeout=0

    for d in $LIBGSCSP/tst/!(bindings|common)/; do
        echo "-------------------------- " 2>&1 | tee -a $MUTANT_LOGFILE
        echo "Compiling mutant for test case "$d 2>&1 | tee -a $MUTANT_LOGFILE

        cd $d

        time ./waf build --notests 2>&1 | tee -a $MUTANT_LOGFILE
         if [ ${PIPESTATUS[0]} -eq 1 ]; then
                echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
                echo $mutant_name"      not compiled" 2>&1 | tee -a $LOGFILE
                compilation_success=0
                break
        fi

        echo "Running test case" 2>&1 | tee -a $MUTANT_LOGFILE
        timeout $TIMEOUT ./waf --alltests 2>&1 | tee -a $MUTANT_LOGFILE

        RET_CODE=${PIPESTATUS[0]}
        
        if [ $RET_CODE -eq 1 -o $RET_CODE -eq 2 ]; then
                end_time="$(date -u +%s)"
                elapsed="$(($end_time-$start_time))"
                mutant_killed=1

                echo "Mutant killed" 2>&1 | tee -a $MUTANT_LOGFILE
                echo $mutant_name"      killed by ${d//$LIBGSCSP/};       time: $elapsed [s]" 2>&1 | tee -a $LOGFILE
                break
        elif [ $RET_CODE -eq 124 ]; then
                mutant_timeout=1
                end_time="$(date -u +%s)"
                elapsed="$(($end_time-$start_time))"
                echo "Mutant timeout" 2>&1 | tee -a $MUTANT_LOGFILE
                echo $mutant_name"      timeout by ${d//$LIBGSCSP/};       time: $elapsed [s]" 2>&1 | tee -a $LOGFILE
                break
        fi
    done

    end_time="$(date -u +%s)"
    elapsed="$(($end_time-$start_time))"


    if [ $mutant_killed -eq 0 -a $compilation_success -eq 1 -a $mutant_timeout -eq 0 ]; then
            echo $mutant_name"      not killed;     time: $elapsed [s]" 2>&1 | tee -a $LOGFILE
    fi

    echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
    cd $LIBGSCSP
    mv $filename_orig.orig $filename_orig

done
