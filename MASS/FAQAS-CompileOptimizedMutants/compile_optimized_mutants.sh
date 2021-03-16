#!/bin/bash

PROJ=$1
PROJ_SRC=$2
PROJ_BUILD=$3
PROJ_ORIGINAL_BUILD=$4
FLAG=$5
SRC_MUTANTS=$6
COMPILED=$7
EXEC_DIR=$8

LOGFILE=$EXEC_DIR/main.log
mkdir -p $EXEC_DIR
touch $LOGFILE

if [ ! -f "$PROJ_ORIGINAL_BUILD" ]; then

    sed "s/TCE/$FLAG/g" ${ORIGINAL_MAKEFILE}.template > ${ORIGINAL_MAKEFILE}
    cat $ORIGINAL_MAKEFILE

    cd $PROJ
    eval "${TCE_COMPILE_CMD[*]}" 2>&1 | tee -a $LOGFILE
    mkdir -p original_build
    cp $PROJ_BUILD/$COMPILED $PROJ/original_build
fi


ORIGINAL_HASH=$EXEC_DIR/original_hash
touch $ORIGINAL_HASH

original_hash=$(sha512sum -b $PROJ_ORIGINAL_BUILD | awk -F' ' '{print $1}')
echo original hash is $original_hash

echo "${original_hash}" > $ORIGINAL_HASH

HASHES=$EXEC_DIR/hashes.csv
NOT_COMPILED=$EXEC_DIR/notcompiled.csv

touch $HASHES $NOT_COMPILED

shopt -s extglob
trap "exit" INT

count=0

for i in $(find $SRC_MUTANTS -name '*.c');do
    start_time=$(($(date +%s%N)/1000000))

    file_wo_opt=${i//$SRC_MUTANTS/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"

    file_wo_mut_end=.${file_wo_opt%%.*}.c

    filename="$(basename -- ${file_wo_mut_end%.*})"

    filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g" | sed "s:./::")

    echo "------------------------------------" 2>&1 | tee -a $LOGFILE
    echo "Mutant: "$i 2>&1 | tee -a $LOGFILE

    cd $PROJ

    # replacing mutant by original source
    echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $LOGFILE
    cp $filename_orig $filename_orig.orig

    echo cp $i $filename_orig 2>&1 | tee -a $LOGFILE
    cp $i $filename_orig

    eval "${TCE_COMPILE_CMD[*]}" 2>&1 | tee -a $LOGFILE
    
    RET_CODE=${PIPESTATUS[0]}
    if [ $RET_CODE -gt 0 ]; then
        echo "Error: mutant could not be compiled" 
        echo $mutant_name"      not compiled" 2>&1 | tee -a $LOGFILE
        
        echo "${mutant_name};${filename_orig}" >> $NOT_COMPILED
    else
        echo "Success: mutant compiled"
        echo $mutant_name"      compiled" 2>&1 | tee -a $LOGFILE
            
        hash=$(sha512sum -b $PROJ_BUILD/$COMPILED | awk -F' ' '{print $1}')
        echo "${mutant_name};${filename_orig};${hash}" >> $HASHES
    fi

    echo "Replacing original source "$i 2>&1 | tee -a $LOGFILE
    cd $PROJ
    mv $filename_orig.orig $filename_orig
   
    touch $filename_orig
 
    if [ $count -eq 10000000 ];then
        break
    else
        count=$((count+1))
    fi

    end_time=$(($(date +%s%N)/1000000))
    elapsed="$(($end_time-$start_time))"

    echo "elapsed time $elapsed [ms]" | tee -a $LOGFILE

done
