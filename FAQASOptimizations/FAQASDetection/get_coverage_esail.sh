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

mkdir -p ${MUTANT_COVERAGE_FOLDER}

shopt -s nullglob;

for i in $(find $SRC_MUTANTS -name '*.c');do

    file_wo_opt=${i//$SRC_MUTANTS/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"
    file_wo_mut_end=.${file_wo_opt%%.*}.c
    filename="$(basename -- ${file_wo_mut_end%.*})"
    function=$(echo $mutant_name | sed 's/.*\.//g')
    filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")
    location_orig=$(dirname $filename_orig)

    if [ -n "$PREFIX" ];then
        location_prefix=$(echo $location_orig | sed -e "s:\.:\.\/$PREFIX:g")
    else
        location_prefix=$location_orig
    fi

    line_number=$(echo $mutant_name | awk -F'.' '{print $3}')
    mutant_key="${mutant_name}|${location_prefix}"

    if grep -Fxq $mutant_key $MUTANT_LIST;then
        start_time=$(($(date +%s%N)/1000000))  
        
        mutant_path=$EXEC_DIR/$mutant_name                                                                                              
        mkdir -p $mutant_path

        MUTANT_LOGFILE=$mutant_path/mutant.log
        touch $MUTANT_LOGFILE
        
        echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
        echo "Mutant: "$i 2>&1 | tee -a $MUTANT_LOGFILE
        
        cd $PROJ

        if [ -n "$PREFIX" ];then
            mutant_cov_name=$(echo $filename_orig | sed -e "s:\.\/:$PREFIX\/:g")
        else
            mutant_cov_name=$(echo $filename_orig | sed -e "s:\.\/::g")
        fi
        
        find . -name '*.gc*' -delete

        # replacing mutant by original source
        cd $PROJ_SRC
    
        echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
        cp $filename_orig $filename_orig.orig

        echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
        cp $i $filename_orig

        exec_loc=$(find $MUTANTS_TRACES -name 'main.csv' | xargs grep --with-filename -m 1 "${mutant_name};${location_orig}" | awk -F':' '{print $1}' | xargs dirname)          
        
        exec_loc_cov=${MUTANTS_RUN}/${exec_loc//$MUTANTS_TRACES/}
        
        echo $exec_loc_cov 2>&1 | tee -a $MUTANT_LOGFILE
        tar xzf $exec_loc_cov/$mutant_name/coverage.gz
        
        cd coverage
        for f in *.tar.gz;do 
            tst="${f%.*.*}"
            mkdir $tst
            tar -xzf "$f"
            mv *.gc* $tst
        done       
 
        find . -name '*.gc*' -exec cp --parents {} $PROJ_TST \;
        cd ../ && rm -rf coverage/

        echo "calculating coverage..."  2>&1 | tee -a $MUTANT_LOGFILE
        source $COV_SCRIPT $mutant_cov_name $mutant_name >> $LOGFILE
    
        find . -name 'coverage.txt' -exec cp --parents {} $MUTANT_COVERAGE_FOLDER \;                                               
        
        echo "Replacing original source " 2>&1 | tee -a $MUTANT_LOGFILE
        cd $PROJ_SRC
        mv $filename_orig.orig $filename_orig
        touch $filename_orig
        
        pushd $MUTANT_COVERAGE_FOLDER
        for tst in */;do
            echo $tst  2>&1 | tee -a $MUTANT_LOGFILE
           
            mutant_start_time=$(($(date +%s%N)/1000000))
 
            RESULTS_COV=${MUTANT_COVERAGE_FOLDER}/${tst}coverage.txt
            
            if [ ! -e $RESULTS_COV ] || ! grep -Fq $mutant_cov_name $RESULTS_COV;then
                continue
            fi
            
            grep -F $mutant_cov_name $RESULTS_COV 2>&1 | tee -a $MUTANT_LOGFILE
            
            echo -n "${mutant_cov_name};${mutant_name};${tst};" >> $LOGFILE

            $PYTHON -u $DIST_SCRIPT --name "$mutant_cov_name" --cov "$RESULTS_COV" --result "$LOGFILE" | tee -a $MUTANT_LOGFILE
            mutant_end_time=$(($(date +%s%N)/1000000))
            mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"
            
            echo -ne ";${mutant_elapsed}\n" >> $LOGFILE
        done
        popd

        rm -rf $MUTANT_COVERAGE_FOLDER/*

        # reset coverage information
        find $PROJ_TST -name '*.gc*' -delete  
        find $PROJ_TST -name 'coverage.txt' -delete  

        end_time=$(($(date +%s%N)/1000000))
        elapsed="$(($end_time-$start_time))"
        echo "elapsed time $elapsed [ms]"  2>&1 | tee -a $MUTANT_LOGFILE

    else
        continue
    fi
done 

