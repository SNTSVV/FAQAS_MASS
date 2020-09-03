#!/bin/bash

HOME=/home/mlfs
PROJ=$HOME/mlfs
PROJ_SRC=$PROJ/libm
PROJ_TST=$HOME/unit-test-suite

BLTS=$HOME/blts
BLTS_APP=$HOME/blts_install/bin/blts_app
ORIGINAL_REPORTS=$HOME/unit-reports
WORKSPACE=$HOME/blts_workspace
SRC_MUTANTS=/opt/mutations/src-mutants
EXEC_DIR=$HOME/test_runs

################################################################################
# Executes command with a timeout
# Params:
#   $1 timeout in seconds
#   $2 command
# Returns 1 if timed out 0 otherwise
ctimeout() {

    time=$1

    # start the command in a subshell to avoid problem with pipes
    # (spawn accepts one command)
    command="/bin/sh -c \"$2\""

    expect -c "set echo \"-noecho\"; set timeout $time; spawn -noecho $command; expect timeout { exit 1 } eof { exit 0 }"    

    if [ $? = 1 ] ; then
#        echo "Timeout after ${time} seconds"
		return 124
    else
		return 0
	fi
}



LOGFILE=$EXEC_DIR/main.csv
mkdir -p $EXEC_DIR
touch $LOGFILE

shopt -s extglob
trap "exit" INT

for i in $(find $SRC_MUTANTS -name '*.c' | shuf -n 2);do
	start_time=$(($(date +%s%N)/1000000))

    file_wo_opt=${i//$SRC_MUTANTS/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"                                                                   

    file_wo_mut_end=.${file_wo_opt%%.*}.c

    filename="$(basename -- ${file_wo_mut_end%.*})"

    function=$(echo $mutant_name | sed 's/.*\.//g')

    filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")

    mutant_path=$EXEC_DIR/$mutant_name
	mkdir -p $mutant_path
	mkdir -p $mutant_path/coverage
	
	MUTANT_LOGFILE=$mutant_path/mutant.log

	touch $MUTANT_LOGFILE

	echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
    echo "Mutant: "$i 2>&1 | tee -a $MUTANT_LOGFILE

    cd $PROJ_SRC

    # replacing mutant by original source
    echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
    cp $filename_orig $filename_orig.orig

    echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
    cp $i $filename_orig

    cd $PROJ

	echo "Building mutant" 2>&1 | tee -a $MUTANT_LOGFILE
	
	make all ARCH=host EXTRA_CFLAGS="-DNDEBUG" 2>&1 | tee -a $MUTANT_LOGFILE
	make all COVERAGE="true" ARCH=host_cov EXTRA_CFLAGS="-DNDEBUG" 2>&1 | tee -a $MUTANT_LOGFILE
	COMP_RET_CODE=${PIPESTATUS[0]}

	if [ $COMP_RET_CODE -eq 1 ]; then
		echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
		mutant_end_time=$(($(date +%s%N)/1000000))
		mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

		echo -ne "${mutant_name};STUBBORN;-;-;-;${mutant_elapsed}\n" >> $LOGFILE
		break
	fi

	#prepare workspace
	echo "Preparing workspace" 2>&1 | tee -a $MUTANT_LOGFILE
	
	cd $BLTS/BLTSConfig
	make install INSTALL_PATH="/home/mlfs/blts_install" 2>&1 | tee -a $MUTANT_LOGFILE

	cd $WORKSPACE
	$BLTS_APP --init 2>&1 | tee -a $MUTANT_LOGFILE

	for tst in $(find $PROJ_TST -name '*.xml');do
		mutant_start_time=$(($(date +%s%N)/1000000))
		cd $WORKSPACE

		echo "#######################" 2>&1 | tee -a $MUTANT_LOGFILE

		tst_filename=$(basename -- $tst)
		tst_filename_wo_xml=${tst_filename//.xml/}
		
		echo "Running test case "$tst 2>&1 | tee -a $MUTANT_LOGFILE
		echo -n "${mutant_name};COMPILED;${tst};" >> $LOGFILE
		
		ctimeout 60 "$BLTS_APP -gcrx $tst_filename_wo_xml -b coverage --nocsv -s $tst" 2>&1 | tee -a $MUTANT_LOGFILE 

		EXEC_RET_CODE=${PIPESTATUS[0]}
		
		mutant_end_time=$(($(date +%s%N)/1000000))
		mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

		echo "Test return code: [$EXEC_RET_CODE]" 2>&1 | tee -a $MUTANT_LOGFILE
        if [ $EXEC_RET_CODE -gt 0 ];then
            echo "Mutant timeout by $tst" 2>&1 | tee -a $MUTANT_LOGFILE

            echo -ne "TIMEOUT;KILLED_${EXEC_RET_CODE};${mutant_elapsed}\n" >> $LOGFILE
		else
			echo "backing up test case coverage" 2>&1 | tee -a $MUTANT_LOGFILE
			GZIP=-9 tar czf ${tst_filename_wo_xml}.tar.gz $tst_filename_wo_xml/Reports
			mv ${tst_filename_wo_xml}.tar.gz $mutant_path/coverage
	
			summaryreport=$tst_filename_wo_xml/Reports/SessionSummaryReport.xml
			originalreport=$ORIGINAL_REPORTS/$summaryreport

			test_cases_failed=$(xmllint --xpath "//report_summary/test_set_summary/test_cases_failed/text()" $summaryreport)
			o_test_cases_failed=$(xmllint --xpath "//report_summary/test_set_summary/test_cases_failed/text()" $originalreport)

			echo "comparing with original execution" 2>&1 | tee -a $MUTANT_LOGFILE
			echo $test_cases_failed $o_test_cases_failed 2>&1 | tee -a $MUTANT_LOGFILE

			if [ "$test_cases_failed" != "$o_test_cases_failed" ]; then
            	echo "Mutant killed by $tst" 2>&1 | tee -a $MUTANT_LOGFILE

            	echo -ne "FAILED;KILLED;${mutant_elapsed}\n" >> $LOGFILE
        	else
            	echo -ne "PASSED;-;${mutant_elapsed}\n" >> $LOGFILE
        	fi
		fi

		echo "removing test data" 2>&1 | tee -a $MUTANT_LOGFILE
		rm -rf $tst_filename_wo_xml
		
	done

	echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
	cd $PROJ_SRC
	mv $filename_orig.orig $filename_orig
	touch $filename_orig

	echo "Deleting test data" 2>&1 | tee -a $MUTANT_LOGFILE 
	rm -rf $WORKSPACE/*

	end_time=$(($(date +%s%N)/1000000))
    elapsed="$(($end_time-$start_time))"

    echo "elapsed time $elapsed [ms]"
done
