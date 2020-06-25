#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

SRC_MUTANTS=/opt/mutations_asn/src-mutants

GRAMMAR=/home/asn/grammar

EXEC_NAME=test_run_`date +"%Y%m%d%H%M"`
EXEC_DIR=/home/asn/test_runs/$EXEC_NAME

LOGFILE=$EXEC_DIR/$EXEC_NAME.log
mkdir -p $EXEC_DIR

touch $LOGFILE

TIMEOUT=10

#for i in `find $SRC_MUTANTS -name '*.c' | shuf -n 5`; do
for i in `find $SRC_MUTANTS -name 'test.mut.intvar_for_646.43523.uoi.c'`; do
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

	cd $GRAMMAR

	# replacing mutant by original source
	echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
	cp $filename_orig $filename_orig.orig

	echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
	cp $i $filename_orig

	# compile mutant
	make clean
	make 2>&1 | tee -a $MUTANT_LOGFILE
	RET_CODE=${PIPESTATUS[0]}

	if [ $RET_CODE -gt 0 ]; then
		echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
		echo $mutant_name"      not compiled" 2>&1 | tee -a $LOGFILE
		
		echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
		mv $filename_orig.orig $filename_orig

		continue
	fi

	# run tests
	timeout $TIMEOUT make coverage 2>&1 | tee -a $MUTANT_LOGFILE
	RET_CODE=${PIPESTATUS[0]}
	
	mutant_killed=0
	mutant_timeout=0

	if [ $RET_CODE -eq 124 ]; then
		end_time="$(date -u +%s)"
		elapsed="$(($end_time-$start_time))"
		
		echo "Mutant timeout" 2>&1 | tee -a $MUTANT_LOGFILE
		echo $mutant_name"          timeout;       time: $elapsed [s]" 2>&1 | tee -a $LOGFILE
		mutant_timeout=1
	elif [ $RET_CODE -gt 0 ];then 
		end_time="$(date -u +%s)"
                elapsed="$(($end_time-$start_time))"

                echo "Mutant killed" 2>&1 | tee -a $MUTANT_LOGFILE
                echo $mutant_name"      killed;       time: $elapsed [s]" 2>&1 | tee -a $LOGFILE
		
		mutant_killed=1
	fi

	end_time="$(date -u +%s)"
	elapsed="$(($end_time-$start_time))"

	if [ $mutant_killed -eq 0 -a $mutant_timeout -eq 0 ]; then
		echo "Mutant not killed" 2>&1 | tee -a $MUTANT_LOGFILE
		echo $mutant_name"      not killed;     time: $elapsed [s]" 2>&1 | tee -a $LOGFILE
	fi

	echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
	mv $filename_orig.orig $filename_orig
	
done
