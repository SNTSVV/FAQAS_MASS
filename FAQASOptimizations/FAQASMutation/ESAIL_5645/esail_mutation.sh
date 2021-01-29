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
#i=$(find $SRC_MUTANTS -name '*.c')

for i in $(find $SRC_MUTANTS -name '*.c');do
        file_wo_opt=${i//$SRC_MUTANTS/}
        mutant_name="$(basename -- ${file_wo_opt%.*})"                                                                   
        file_wo_mut_end=.${file_wo_opt%%.*}.c
        filename="$(basename -- ${file_wo_mut_end%.*})"

        function=$(echo $mutant_name | sed 's/.*\.//g')

        filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")

        location_orig=$(dirname $filename_orig)

        mutant_line=$(echo $mutant_name | awk -F'.' '{print $3}')
        
        echo $filename $mutant_line

        TESTCASES=$(find $LIST_TESTS -name "${filename}.c.${mutant_line}.coverage.txt")

        if ! grep -Fq "5645" $TESTCASES;then
            echo "$i 5645 not included"
            continue
        fi
        
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

        while IFS="" read -r p || [ -n "$p" ];do
            mutant_start_time=$(($(date +%s%N)/1000000))
            
            # obtaining test number to be executed
            read tst <<< $(awk -F'/' '{print $5}' <<< "$p")

            if [ "$tst" -ne "5645" ]; then
                continue
            fi

            # obtaining corresponding timeout for the test case
            TIMEOUT=$(grep "^${tst};" $TIMES | awk -F';' '{$2=($2*4)/1000; printf("%.0f\n", $2);}')

            echo "Running test case "$tst 2>&1 | tee -a $MUTANT_LOGFILE
            echo -n "${mutant_name};${location_orig};COMPILED;${tst};" >> $LOGFILE

            timeout $TIMEOUT $ESAIL --obsw $PROJ_SRC/_binaries/OBSW.exe -n -c --fast --source $PROJ_SRC --version 04010000 -t $tst &
            ESAIL_PID=$!

            wait $ESAIL_PID
            EXEC_RET_CODE=$?
         
            mutant_end_time=$(($(date +%s%N)/1000000))
            mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"
            
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
            
        done < $TESTCASES

        echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
        cd $PROJ_SRC
        mv $filename_orig.orig $filename_orig
        touch $filename_orig

        end_time=$(($(date +%s%N)/1000000))
        elapsed="$(($end_time-$start_time))"

        echo "elapsed time $elapsed [ms]"
done
