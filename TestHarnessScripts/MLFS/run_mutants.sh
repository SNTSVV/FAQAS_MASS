#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

SRC_MUTANTS=/opt/mutations/fake

TST=/home/mlfs/partial
BLTS=/home/mlfs/blts
BLTS_APP=/home/mlfs/blts_install/bin/blts_app
WORKSPACE=/home/mlfs/blts_workspace
ORIGINAL_REPORTS=/home/mlfs/unit-reports

MLFS=/home/mlfs/mlfs

EXEC_NAME=test_run_$(date +"%Y%m%d%H%M")
EXEC_DIR=/home/mlfs/test_runs/$EXEC_NAME

LOGFILE=$EXEC_DIR/$EXEC_NAME.log
mkdir -p $EXEC_DIR

touch $LOGFILE

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

	cd $MLFS/libm

	# replacing mutant by original source
        echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
        cp $filename_orig $filename_orig.orig

	echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
	cp $i $filename_orig
	
	cd $MLFS

	# compile mutant
	make all ARCH=host 2>&1 | tee -a $MUTANT_LOGFILE
	make all COVERAGE="true" ARCH=host_cov 2>&1 | tee -a $MUTANT_LOGFILE
	RET_CODE=$?

	if [ $RET_CODE -gt 0 ]; then
		echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
                echo $mutant_name"      not compiled" 2>&1 | tee -a $LOGFILE
		break
	fi
	
	cd $BLTS/BLTSConfig
	make install	
	
	cd $WORKSPACE
	$BLTS_APP --init
	
	possible_match=`echo $filename | sed 's/.*_//'` 

	mutant_killed=0
	for tst in `find $TST -name "*${possible_match}*.xml" && find $TST -mindepth 1 ! -name "*${possible_match}*.xml"`;do

		cd $WORKSPACE
		
		echo "-------------------------- " 2>&1 | tee -a $MUTANT_LOGFILE

		tst_filename=`basename -- $tst`
		tst_filename_wo_xml=${tst_filename//.xml/}
		
		echo "Running test case "$tst 2>&1 | tee -a $MUTANT_LOGFILE
		$BLTS_APP -gcrx $tst_filename_wo_xml -b coverage --nocsv -s $tst 2>&1 | tee -a $MUTANT_LOGFILE
		
		summaryreport=$tst_filename_wo_xml/Reports/SessionSummaryReport.xml
		originalreport=$ORIGINAL_REPORTS/$summaryreport

		test_cases_failed=`xmllint --xpath "//report_summary/test_set_summary/test_cases_failed/text()" $summaryreport`	
		o_test_cases_failed=`xmllint --xpath "//report_summary/test_set_summary/test_cases_failed/text()" $originalreport`	
			
		echo $test_cases_failed $o_test_cases_failed 2>&1 | tee -a $MUTANT_LOGFILE

		echo "removing test data" 2>&1 | tee -a $MUTANT_LOGFILE
		rm -rf $tst_filename_wo_xml

		if [ "$test_cases_failed" != "$o_test_cases_failed" ]; then
			end_time="$(date -u +%s)"
                        elapsed="$(($end_time-$start_time))"

			echo "Mutant killed" 2>&1 | tee -a $MUTANT_LOGFILE 
			echo $mutant_name"      killed by $tst_filename_wo_xml;       time: $elapsed [s]" 2>&1 | tee -a $LOGFILE	
			mutant_killed=1
			break
		fi
	

		#remove
		#break
	done

	end_time="$(date -u +%s)"
        elapsed="$(($end_time-$start_time))"

	if [ $mutant_killed -eq 0 ]; then
                echo $mutant_name"      not killed;     time: $elapsed [s]" 2>&1 | tee -a $LOGFILE
        fi

	echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
        cd $MLFS/libm
  	mv $filename_orig.orig $filename_orig

	echo "Deleting test data" 2>&1 | tee -a $MUTANT_LOGFILE
	#rm -rf $WORKSPACE/*
	#`find $WORKSPACE -maxdepth 1 -mindepth 1 -type d -exec rm -rf "{}" \;`		
	
	#remove
	#break
done
