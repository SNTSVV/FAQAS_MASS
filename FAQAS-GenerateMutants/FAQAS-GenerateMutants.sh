#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

LOGFILE=$MUTANTS_DIR/mutation_$(date +"%Y%m%d%H%M").log
touch $LOGFILE

start_time="$(date -u +%s)"

jq -c '.[]' $MUTANTS_DIR/compile_commands.json | while read i; do
    FILE=$(echo $i | jq -r '.file')
    
    if [ ! -z $COVERAGE_NOT_INCLUDE ];then
         file_check=$(echo $FILE | grep "$PROJ" | grep -v "$COVERAGE_NOT_INCLUDE")
   
         if [ -z "$file_check" ] || [ ! -f "$file_check" ];then
            continue
         fi
     fi
    ARGS=$(echo $i | jq '.command')
    
    if [ "$ARGS" == "null" ];then
        ARGS=$(echo $i | jq '.arguments | join(" ")')
    fi
    
    DIR=$(echo $i | jq -r '.directory')

    echo "-----------------------" 2>&1 | tee -a $LOGFILE
    echo "Mutating "$FILE 2>&1 | tee -a $LOGFILE

    cd $DIR

    path_wo_ext="${FILE%.*}"
    path_wo_root="${path_wo_ext#$PROJ/}"

    mkdir -p $MUTANTS_DIR/$path_wo_root

    orig_dir=$(dirname -- "$FILE")

    ARGS_wo_quotes=$(sed -e 's/^"//' -e 's/"$//' <<<"$ARGS")

    $SRCIROR_COMPILER --compilation "$ARGS_wo_quotes" 2>&1 | tee -a $LOGFILE
    rsync -av --no-p --no-g --remove-source-files ${orig_dir}/*.mut.* $MUTANTS_DIR/$path_wo_root

done

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"

echo "elapsed time: $elapsed [s]"  2>&1 | tee -a $LOGFILE
