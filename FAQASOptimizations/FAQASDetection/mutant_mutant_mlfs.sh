#!/bin/bash

PROJ=$1
PROJ_SRC=$2
PROJ_TST=$3
PROJ_COV=$4
SRC_MUTANTS=$5
MUTANTS_RUN=$6
COV_SCRIPT=$7
DIST_SCRIPT=$8
EXEC_DIR=$9
MUTANT_LIST=${10}
RESULTS=${11}
PYTHON=${12}
MUTANT_COVERAGE_FOLDER=${13}
MUTANTS_TRACES=${14}
PREFIX=${15}

LOGFILE=$EXEC_DIR/main.csv
mkdir -p $EXEC_DIR
touch $LOGFILE

mkdir -p ${MUTANT_COVERAGE_FOLDER}_1
mkdir -p ${MUTANT_COVERAGE_FOLDER}_2

shopt -s nullglob;

mutants_array=($(find $SRC_MUTANTS -name '*.c')) 

for (( it = 0; it < "${#mutants_array[@]}"; it++ ));do

    m_1=${mutants_array[$it]}
    
    file_wo_opt_1=${m_1//$SRC_MUTANTS/}
    mutant_name_1="$(basename -- ${file_wo_opt_1%.*})"                                                                   
    file_wo_mut_end_1=.${file_wo_opt_1%%.*}.c
    filename_1="$(basename -- ${file_wo_mut_end_1%.*})"
    function_1=$(echo $mutant_name_1 | sed 's/.*\.//g')
    filename_orig_1=$(echo $file_wo_mut_end_1 | sed -e "s/\(.*\)$filename_1\//\1/g")
    location_orig_1=$(dirname $filename_orig_1)

    line_number_1=$(echo $mutant_name_1 | awk -F'.' '{print $3}')

    if grep -Fxq $mutant_name_1 $MUTANT_LIST;then
    
        start_time=$(($(date +%s%N)/1000000))  
        
        mutant_path=$EXEC_DIR/$mutant_name_1
        mkdir -p $mutant_path

        MUTANT_LOGFILE=$mutant_path/mutant.log
        touch $MUTANT_LOGFILE
        
        echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
        echo "Mutant A: "$m_1 2>&1 | tee -a $MUTANT_LOGFILE
        
        # replacing mutant by original source
        cd $PROJ_SRC
    
        echo cp $filename_orig_1 $filename_orig_1.orig 2>&1 | tee -a $MUTANT_LOGFILE
        cp $filename_orig_1 $filename_orig_1.orig

        echo cp $m_1 $filename_orig_1 2>&1 | tee -a $MUTANT_LOGFILE
        cp $m_1 $filename_orig_1

        exec_loc_1=$(find $MUTANTS_TRACES -name 'main.csv' | xargs grep -m 1 "${mutant_name_1}" | awk -F':' '{print $1}' | xargs dirname)                                                                                                                           
        exec_loc_cov_1=${MUTANTS_RUN}/${exec_loc_1//$MUTANTS_TRACES/}
        
        echo $exec_loc_cov_1 2>&1 | tee -a $MUTANT_LOGFILE
        tar xzf $exec_loc_cov_1/$mutant_name_1/coverage.gz
 
        cd coverage
        for f in *.tar.gz;do tar -xzf "$f";done

        find . -name '*.gc*' -exec cp --parents {} $PROJ_TST \;

        source $COV_SCRIPT
   
        cd $PROJ_TST
 
        find . -name 'coverage.txt' -exec cp --parents {} ${MUTANT_COVERAGE_FOLDER}_1 \;

        echo "Replacing original source " 2>&1 | tee -a $MUTANT_LOGFILE
        cd $PROJ_SRC
        mv $filename_orig_1.orig $filename_orig_1
        touch $filename_orig_1

        # reset coverage information
        find $PROJ_TST -name '*.gc*' -delete  
        find $PROJ_TST -name 'coverage.txt' -delete  

        ################
        for ((jt = it + 1; jt < ${#mutants_array[@]}; jt++)); do
            
            m_2=${mutants_array[$jt]}

            file_wo_opt_2=${m_2//$SRC_MUTANTS/}
            mutant_name_2="$(basename -- ${file_wo_opt_2%.*})"                                                                   
            file_wo_mut_end_2=.${file_wo_opt_2%%.*}.c
            filename_2="$(basename -- ${file_wo_mut_end_2%.*})"
            function_2=$(echo $mutant_name_2 | sed 's/.*\.//g')
            filename_orig_2=$(echo $file_wo_mut_end_2 | sed -e "s/\(.*\)$filename_2\//\1/g")
            location_orig_2=$(dirname $filename_orig_2)

#            if [[ $mutant_name_2 != *"watchdog.mut.118.7_1_9.ROR.gs_swwd_destroy"* ]];then
#                continue
#            fi

            line_number_2=$(echo $mutant_name_2 | awk -F'.' '{print $3}')

            if grep -Fxq $mutant_name_2 $MUTANT_LIST;then

                echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
                echo "Mutant B: "$m_2 2>&1 | tee -a $MUTANT_LOGFILE
                
                # replacing mutant by original source
                cd $PROJ_SRC
            
                echo cp $filename_orig_2 $filename_orig_2.orig 2>&1 | tee -a $MUTANT_LOGFILE
                cp $filename_orig_2 $filename_orig_2.orig

                echo cp $m_2 $filename_orig_2 2>&1 | tee -a $MUTANT_LOGFILE
                cp $m_2 $filename_orig_2
            
                exec_loc_2=$(find $MUTANTS_TRACES -name 'main.csv' | xargs grep -m 1 "${mutant_name_2}" | awk -F':' '{print $1}' | xargs dirname)   
                exec_loc_cov_2=${MUTANTS_RUN}/${exec_loc_2//$MUTANTS_TRACES/}
        
                echo $exec_loc_cov_2 2>&1 | tee -a $MUTANT_LOGFILE
                tar xzf $exec_loc_cov_2/$mutant_name_2/coverage.gz
                
                cd coverage
                for f in *.tar.gz;do tar -xzf "$f";done           
     
                find . -name '*.gc*' -exec cp --parents {} $PROJ_TST \;

                source $COV_SCRIPT

                cd $PROJ_TST
            
                find . -name 'coverage.txt' -exec cp --parents {} ${MUTANT_COVERAGE_FOLDER}_2 \;

                echo "Replacing original source " 2>&1 | tee -a $MUTANT_LOGFILE
                cd $PROJ_SRC
                mv $filename_orig_2.orig $filename_orig_2
                touch $filename_orig_2

                # reset coverage information
                find $PROJ_TST -name '*.gc*' -delete  
                find $PROJ_TST -name 'coverage.txt' -delete  

                pushd ${MUTANT_COVERAGE_FOLDER}_1
                for tst in */;do
                    echo $tst  2>&1 | tee -a $MUTANT_LOGFILE
                   
                    mutant_start_time=$(($(date +%s%N)/1000000))
                    
                    mutant_cov_name=$(echo $filename_orig_1 | sed -e "s:\.\/:$PREFIX\/:g")
                    
                    if [ -n "$PREFIX" ];then
                        mutant_cov_name=$(echo $filename_orig_1 | sed -e "s:\.\/:$PREFIX\/:g")
                    else
                        mutant_cov_name=$(echo $filename_orig_1 | sed -e "s:\.\/::g")
                    fi

                    echo $mutant_cov_name  

                    RESULTS_COV_1=${MUTANT_COVERAGE_FOLDER}_1/${tst}Reports/Coverage/Data/coverage.txt
                    RESULTS_COV_2=${MUTANT_COVERAGE_FOLDER}_2/${tst}Reports/Coverage/Data/coverage.txt
                    
                    if [ ! -e $RESULTS_COV_1 ] || ! grep -Fq $mutant_cov_name $RESULTS_COV_1 ;then
                        continue
                    fi

                    if [ ! -e $RESULTS_COV_2 ] || ! grep -Fq $mutant_cov_name $RESULTS_COV_2 ;then
                        continue
                    fi 

                    grep $mutant_cov_name $RESULTS_COV_1  2>&1 | tee -a $MUTANT_LOGFILE
                    grep $mutant_cov_name $RESULTS_COV_2  2>&1 | tee -a $MUTANT_LOGFILE
                    
                    echo -n "${mutant_cov_name};${mutant_name_1};${mutant_name_2};${tst};" >> $LOGFILE

                    $PYTHON -u $DIST_SCRIPT --name "$mutant_cov_name" --cov_a "$RESULTS_COV_1" --cov_b "$RESULTS_COV_2" --result "$LOGFILE" --line_a "$line_number_1" --line_b "$line_number_2" | tee -a $MUTANT_LOGFILE

                    mutant_end_time=$(($(date +%s%N)/1000000))
                    mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"
                    
                    echo -ne ";${mutant_elapsed}\n" >> $LOGFILE
                done
                popd

                rm -rf ${MUTANT_COVERAGE_FOLDER}_2/*
            fi
            # reset coverage information
            find $PROJ_TST -name '*.gc*' -delete
            find $PROJ_TST -name 'coverage.txt' -delete
        done 

        rm -rf ${MUTANT_COVERAGE_FOLDER}_1/*

        end_time=$(($(date +%s%N)/1000000))
        elapsed="$(($end_time-$start_time))"
        echo "elapsed time $elapsed [ms]"  2>&1 | tee -a $MUTANT_LOGFILE

    else
        continue
    fi
done 

