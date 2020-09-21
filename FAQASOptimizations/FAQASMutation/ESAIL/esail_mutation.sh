#!/bin/bash

HOME=/home/svf
PROJ=$HOME/Obsw/Source
PROJ_SRC=$PROJ
BINARIES=$PROJ/_binaries
OBJECTS=$PROJ/_objects
SRC_MUTANTS=/opt/mutations/src-mutants/m$1
EXEC_DIR=$HOME/test_runs
ESAIL=/home/svf/Obsw/Test/lib/esail.sh

COVERAGE=$PROJ_SRC/coverage
TIMES=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutation/ESAIL/times

LIST_TESTS=/opt/list_tests

TMUX=/home/svf/local2/bin/tmux

LOGFILE=$EXEC_DIR/main.csv
mkdir -p $EXEC_DIR
touch $LOGFILE

#########

shopt -s extglob

cd $HOME

shopt -s expand_aliases
source .bashrc
source .bash_profile

#########

start_time=$(($(date +%s%N)/1000000))

# i is the mutant
i=$(find $SRC_MUTANTS -name '*.c')

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

mutant_line=$(echo $mutant_name | awk -F'.' '{print $3}')

echo $filename $mutant_line

TESTCASES=$(find $LIST_TESTS -name "${filename}.c.${mutant_line}.coverage.txt")

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

make clean	
make target release=true snt_opt=true 2>&1 | tee -a $MUTANT_LOGFILE

COMP_RET_CODE=${PIPESTATUS[0]}

if [ $COMP_RET_CODE -eq 1 ]; then
    echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
    mutant_end_time=$(($(date +%s%N)/1000000))
    mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

    echo -ne "${mutant_name};${location_orig};STUBBORN;-;-;-;${mutant_elapsed}\n" >> $LOGFILE
    exit 1
fi

number_sources_in_folder=$(find $location_orig -type f -name '*.c' | wc -l)

if [ $number_sources_in_folder -gt 1 ];then
    
    touch $filename_orig
    make target release=true snt_opt=true snt_cov=true 

    for src_in_folder in $(find $location_orig -type f -name '*.c' -and -not -name "$filename.c");do
        object=$(basename -- $src_in_folder | sed 's:\.c::')
        rm $OBJECTS/$object.*   
    done

    make target release=true snt_opt=true
else
    touch $filename_orig
    make target release=true snt_opt=true snt_cov=true 
fi

# do not optimize gcov functionalities
touch $PROJ_SRC/Utilities/Gcov/Source/gcc.c
make target release=true 

# setting debugger
port=$((2300+$1))
sed "s/PORT/$port/g" $COVERAGE/coverage.gdb.template > $COVERAGE/coverage.gdb

while IFS="" read -r p || [ -n "$p" ];do
    mutant_start_time=$(($(date +%s%N)/1000000))
    
    # obtaining test number to be executed
    read tst <<< $(awk -F'/' '{print $5}' <<< "$p")

    # obtaining corresponding timeout for the test case
    TIMEOUT=$(grep "^${tst};" $TIMES | awk -F';' '{$2=($2*4)/1000; printf("%.0f\n", $2);}')

    echo "Running test case "$tst 2>&1 | tee -a $MUTANT_LOGFILE
    echo -n "${mutant_name};${location_orig};COMPILED;${tst};" >> $LOGFILE

    timeout $TIMEOUT $ESAIL --obsw $PROJ_SRC/_binaries/OBSW.exe -n -c --fast --source $PROJ_SRC --version 04010000 -t $tst --gdb --coverage --port $port &
    ESAIL_PID=$!

    sleep 8
    echo "starting debugger" | tee -a $MUTANT_LOGFILE
    sleep 2

    $TMUX new-session -d -s "esail-debugger-$1" "source $COVERAGE/debugger.sh"

    wait $ESAIL_PID
    EXEC_RET_CODE=$?
 
    mutant_end_time=$(($(date +%s%N)/1000000))
    mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"
    
    $TMUX kill-session -t "esail-debugger-$1"
    sleep 2

    # backing up 
    pushd $OBJECTS
    GZIP=-9 tar czf ${tst}.tar.gz *.gc*
    mv ${tst}.tar.gz $mutant_path/coverage
    popd

    echo "Test return code: [$EXEC_RET_CODE]" 2>&1 | tee -a $MUTANT_LOGFILE
    if [ $EXEC_RET_CODE -ge 124 ];then                                                                               
        echo "Mutant timeout by $tst" 2>&1 | tee -a $MUTANT_LOGFILE
        echo -ne "TIMEOUT;KILLED_${EXEC_RET_CODE};${mutant_elapsed}\n" >> $LOGFILE

    elif [ $EXEC_RET_CODE -gt 0 ];then
        echo "Mutant killed by $tst" 2>&1 | tee -a $MUTANT_LOGFILE
        echo -ne "FAILED;KILLED;${mutant_elapsed}\n" >> $LOGFILE
    else
        echo -ne "PASSED;-;${mutant_elapsed}\n" >> $LOGFILE
    fi 
    
    echo "removing test data" 2>&1 | tee -a $MUTANT_LOGFILE
    find $OBJECTS -name '*.gcda' -delete
 
done < $TESTCASES

# backing up coverage
pushd $mutant_path
GZIP=-9 tar czf coverage.gz coverage/
rm -rf $mutant_path/coverage
popd

echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
cd $PROJ_SRC
mv $filename_orig.orig $filename_orig
touch $filename_orig

end_time=$(($(date +%s%N)/1000000))
elapsed="$(($end_time-$start_time))"

echo "elapsed time $elapsed [ms]"
