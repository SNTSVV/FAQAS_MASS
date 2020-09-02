#!/bin/bash

PROJ=$1
PROJ_SRC=$2
PROJ_TST=$3
PROJ_COV=$4
SRC_MUTANTS=$5
BUILD_COMMAND=$6
EXECUTION_COMMAND=$7
FILTER_TST=$8
EXEC_DIR=$9
TIMEOUT=${10}

MUTANT=${11}

LOGFILE=$EXEC_DIR/main.csv
mkdir -p $EXEC_DIR
touch $LOGFILE

shopt -s extglob
trap "exit" INT

for i in $(find $SRC_MUTANTS -name "${MUTANT}.c");do
	start_time=$(($(date +%s%N)/1000000))

    file_wo_opt=${i//$SRC_MUTANTS/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"                                                                   

    file_wo_mut_end=.${file_wo_opt%%.*}.c

    filename="$(basename -- ${file_wo_mut_end%.*})"

    function=$(echo $mutant_name | sed 's/.*\.//g')

    filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")

    mutant_path=$EXEC_DIR/$mutant_name
	mkdir -p $mutant_path
	
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

	# set a maximum of usable memory
	ulimit -v 4000000

	mutant_live=1
	for tst in $PROJ_TST/!($FILTER_TST)/;do
		echo "##############################" 2>&1 | tee -a $MUTANT_LOGFILE
		echo "Building mutant for test case "$tst 2>&1 | tee -a $MUTANT_LOGFILE

		mutant_start_time=$(($(date +%s%N)/1000000))

		cd $tst
		
		echo -n "${mutant_name};" >> $LOGFILE

		"${BUILD_COMMAND[@]}" 2>&1 | tee -a $MUTANT_LOGFILE
		COMP_RET_CODE=${PIPESTATUS[0]}

		if [ $COMP_RET_CODE -eq 1 ]; then
			echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
			mutant_end_time=$(($(date +%s%N)/1000000))
		    mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

			mutant_live=0

			echo -ne "STUBBORN;-;-;-;${mutant_elapsed}\n" >> $LOGFILE
			break
		else
			echo -n "COMPILED;${tst};" >> $LOGFILE
		fi
		
		echo "Running test case" 2>&1 | tee -a $MUTANT_LOGFILE
		timeout $TIMEOUT "${EXECUTION_COMMAND[@]}" 2>&1 | tee -a $MUTANT_LOGFILE

		EXEC_RET_CODE=${PIPESTATUS[0]}
	
		mutant_end_time=$(($(date +%s%N)/1000000))
        mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"
		
		echo "Test return code: [$EXEC_RET_CODE]" 2>&1 | tee -a $MUTANT_LOGFILE
		if [ $EXEC_RET_CODE -ge 124 ];then
			echo "Mutant timeout by $tst" 2>&1 | tee -a $MUTANT_LOGFILE
			
			mutant_live=0
			echo -ne "TIMEOUT;KILLED__${EXEC_RET_CODE};${mutant_elapsed}\n" >> $LOGFILE
			break

		elif [ $EXEC_RET_CODE -gt 0 ];then
			echo "Mutant killed by $tst" 2>&1 | tee -a $MUTANT_LOGFILE
			
			mutant_live=0
			echo -ne "FAILED;KILLED;${mutant_elapsed}\n" >> $LOGFILE
			break
		else
			echo -ne "PASSED;-;${mutant_elapsed}\n" >> $LOGFILE
		fi
	done

	echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
	cd $PROJ_SRC
	mv $filename_orig.orig $filename_orig
	touch $filename_orig

	# backing up coverage
	if [ $mutant_live -eq 1 ];then
		cd $PROJ_COV
		echo backing up $filename.gcda | tee -a $MUTANT_LOGFILE
		mkdir -p $mutant_path/coverage
		
		find . -name "*${filename}.*gcda" -exec cp --parents '{}' $mutant_path/coverage \;
		find . -name "*${filename}.*gcno" -exec cp --parents '{}' $mutant_path/coverage \;
		
		cd $mutant_path
		GZIP=-9 tar czf coverage.gz coverage/
		rm -rf $mutant_path/coverage
	fi

	# reset coverage information
	find . -name '*.gcda' -delete
	
	end_time=$(($(date +%s%N)/1000000))
    elapsed="$(($end_time-$start_time))"

    echo "elapsed time $elapsed [ms]"
done

