#!/bin/bash

# loading functions backup_tst_coverage and run_tst_case
. $APP_RUN_DIR/mutation_additional_functions.sh

MUT_EXEC_DIR=$1
MUTANT_ID=$2
COMPILATION_CMD=$3
ADDITIONAL_CMD=$4
ADDITIONAL_CMD_AFTER=$5
TEST_LIST=$6
TIMES=$7

LOGFILE=$MUT_EXEC_DIR/main.csv
mkdir -p $MUT_EXEC_DIR
touch $LOGFILE

start_time=$(($(date +%s%N)/1000000))

mutant_name=$(echo $MUTANT_ID | awk -F'|' '{print $1}')
mutant_location=$(echo $MUTANT_ID | awk -F'|' '{print $2}' | sed "s:\.c::")

original_src=$(echo $MUTANT_ID | awk -F'|' '{print $2}')

mutant_full_path=$(find $MUTANTS_DIR -wholename "*$mutant_location*$mutant_name.c")

mutant_src_name=$(echo $mutant_name | awk -F'.' '{print $1}')

mutant_exec_path=$MUT_EXEC_DIR/$mutant_name
mkdir -p $mutant_exec_path
mkdir -p $mutant_exec_path/coverage
    
MUTANT_LOGFILE=$mutant_exec_path/mutant.log

touch $MUTANT_LOGFILE

echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
echo "Mutant: "$mutant_full_path 2>&1 | tee -a $MUTANT_LOGFILE

cd $PROJ

# replacing mutant by original source
echo cp $original_src $original_src.orig 2>&1 | tee -a $MUTANT_LOGFILE
cp $original_src $original_src.orig

echo cp $mutant_full_path $original_src 2>&1 | tee -a $MUTANT_LOGFILE
cp $mutant_full_path $original_src

for tst in $(echo $TEST_LIST | sed "s/;/ /g");do
    mutant_start_time=$(($(date +%s%N)/1000000))

    echo "Building mutant for test case $tst" 2>&1 | tee -a $MUTANT_LOGFILE

    cd $PROJ_TST/$tst

    eval "${COMPILATION_CMD[@]}" 2>&1 | tee -a $MUTANT_LOGFILE
    COMP_RET_CODE=${PIPESTATUS[0]}

    if [ $COMP_RET_CODE -eq 1 ]; then
        echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
        mutant_end_time=$(($(date +%s%N)/1000000))
        mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

        echo -ne "${mutant_name};${original_src};STILLBORN;-;-;-;${mutant_elapsed}\n" >> $LOGFILE
        break
    fi

    echo -n "${mutant_name};${original_src};COMPILED;" >> $LOGFILE
    
    echo "Additional commands before execution of test case" 2>&1 | tee -a $MUTANT_LOGFILE
    eval "${ADDITIONAL_CMD[@]}" 2>&1 | tee -a $MUTANT_LOGFILE
    
    killed=0

    TIMEOUT=$(grep "^${tst}:" $TIMES | awk -F':' '{print $2}')

    export -f run_tst_case
    timeout $TIMEOUT bash -c "run_tst_case $tst" 2>&1 | tee -a $MUTANT_LOGFILE

    EXEC_RET_CODE=${PIPESTATUS[0]}

    mutant_end_time=$(($(date +%s%N)/1000000))
    mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"
    
    echo "Test return code: [$EXEC_RET_CODE]" 2>&1 | tee -a $MUTANT_LOGFILE
    
    if [ $EXEC_RET_CODE -ge 124 ];then
        echo "Mutant timeout by $tst" 2>&1 | tee -a $MUTANT_LOGFILE

        echo -ne "${tst};TIMEOUT;KILLED_${EXEC_RET_CODE};${mutant_elapsed}\n" >> $LOGFILE

        killed=1
    elif [ $EXEC_RET_CODE -gt 0 ];then
        echo "Mutant killed by $tst" 2>&1 | tee -a $MUTANT_LOGFILE

        echo -ne "${tst};FAILED;KILLED;${mutant_elapsed}\n" >> $LOGFILE

        killed=1
    else
        echo "Mutant live by $tst" 2>&1 | tee -a $MUTANT_LOGFILE
        echo -ne "${tst};PASSED;LIVE;${mutant_elapsed}\n" >> $LOGFILE
    fi 
    
    if [ $EXEC_RET_CODE -lt 124 ];then
        echo "Backing up test case coverage $original_src" 2>&1 | tee -a $MUTANT_LOGFILE
        backup_tst_coverage $tst $mutant_src_name $mutant_exec_path
    fi

    if [ $killed -eq 1 ];then
        break
    fi
done

echo "Backing up mutant coverage" 2>&1 | tee -a $MUTANT_LOGFILE
pushd $mutant_exec_path
GZIP=-9 tar czf coverage.tar.gz coverage/
rm -rf coverage/
popd

echo "Replacing original source" $mutant_full_path 2>&1 | tee -a $MUTANT_LOGFILE
cd $PROJ
mv $original_src.orig $original_src
touch $original_src

# reset coverage information
find $PROJ_TST -name '*.gc*' -delete

echo "Additional commands after execution of tests" 2>&1 | tee -a $MUTANT_LOGFILE
eval "${ADDITIONAL_CMD_AFTER[@]}" 2>&1 | tee -a $MUTANT_LOGFILE

end_time=$(($(date +%s%N)/1000000))
elapsed="$(($end_time-$start_time))"

echo "elapsed time $elapsed [ms]"

if [ $killed -eq 1 ];then
    exit 1
else
    exit 0
fi

