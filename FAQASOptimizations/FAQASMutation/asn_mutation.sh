#!/bin/bash

HOME=/home/asn
PROJ=$HOME/mutation_test
PROJ_SRC=$PROJ
PROJ_TST=$PROJ
SRC_MUTANTS=/opt/mutations/src-mutants/$1
EXEC_DIR=$HOME/test_runs

LOGFILE=$EXEC_DIR/main.csv
mkdir -p $EXEC_DIR
touch $LOGFILE

shopt -s extglob
trap "exit" INT

start_time=$(($(date +%s%N)/1000000))

count=0
for i in $(find $SRC_MUTANTS -name '*.c');do
	mutant_start_time=$(($(date +%s%N)/1000000))
	
    file_wo_opt=${i//$SRC_MUTANTS/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"                                                                   

    file_wo_mut_end=.${file_wo_opt%%.*}.c

    filename="$(basename -- ${file_wo_mut_end%.*})"

    function=$(echo $mutant_name | sed 's/.*\.//g')

	filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")

	location_orig=$(dirname $filename_orig)	

    mutant_path=$EXEC_DIR/$mutant_name
	mkdir -p $mutant_path
	mkdir -p $mutant_path/coverage
	
	MUTANT_LOGFILE=$mutant_path/mutant.log

	touch $MUTANT_LOGFILE

	echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
    echo "Mutant: "$i 2>&1 | tee -a $MUTANT_LOGFILE

    cd $PROJ_SRC

    # replacing mutant by original source
    echo cp ./test.c ./test.c.orig 2>&1 | tee -a $MUTANT_LOGFILE
    cp ./test.c ./test.c.orig

    echo cp $i ./test.c 2>&1 | tee -a $MUTANT_LOGFILE
    cp $i ./test.c

    cd $PROJ

	echo "Building mutant" 2>&1 | tee -a $MUTANT_LOGFILE

	echo -n "${mutant_name};${location_orig};" >> $LOGFILE
	
	make clean
    make 2>&1 | tee -a $MUTANT_LOGFILE
	COMP_RET_CODE=${PIPESTATUS[0]}

	if [ $COMP_RET_CODE -eq 1 ]; then
		echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
		mutant_end_time=$(($(date +%s%N)/1000000))
		mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

		echo -ne "${mutant_name};STUBBORN;-;-;-;${mutant_elapsed}\n" >> $LOGFILE
		break
	else
        echo -n "COMPILED;testsuite;" >> $LOGFILE
    fi
	
	echo "Running test case" 2>&1 | tee -a $MUTANT_LOGFILE
    timeout 10 make coverage 2>&1 | tee -a $MUTANT_LOGFILE
    EXEC_RET_CODE=${PIPESTATUS[0]}

    mutant_end_time=$(($(date +%s%N)/1000000))
    mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

    echo "Test return code: [$EXEC_RET_CODE]" 2>&1 | tee -a $MUTANT_LOGFILE
    if [ $EXEC_RET_CODE -ge 124 ];then
	    echo "Mutant timeout by testsuite" 2>&1 | tee -a $MUTANT_LOGFILE
        echo -ne "TIMEOUT;KILLED_${EXEC_RET_CODE};${mutant_elapsed}\n" >> $LOGFILE

    elif [ $EXEC_RET_CODE -gt 0 ];then
        echo "Mutant killed by testsuite" 2>&1 | tee -a $MUTANT_LOGFILE

        echo -ne "FAILED;KILLED;${mutant_elapsed}\n" >> $LOGFILE

    else
        echo -ne "PASSED;-;${mutant_elapsed}\n" >> $LOGFILE
    fi

    echo "Backing up mutant coverage" 2>&1 | tee -a $MUTANT_LOGFILE
	mkdir -p $mutant_path/coverage
    find . -name "test.*gc*" -exec cp '{}' $mutant_path/coverage \;

    pushd $mutant_path 
    GZIP=-9 tar czf coverage.gz coverage/
    rm -rf coverage/
    popd   

	echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
	cd $PROJ_SRC
	mv ./test.c.orig ./test.c
	touch ./test.c

	if [ $count -eq 1000000000 ];then
		break;
	else
		count=$((count+1))
	fi
done

end_time=$(($(date +%s%N)/1000000))
elapsed="$(($end_time-$start_time))"

echo "elapsed time $elapsed [ms]"
