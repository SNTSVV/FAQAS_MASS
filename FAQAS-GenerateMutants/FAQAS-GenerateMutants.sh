#!/bin/bash

LOGFILE=$MUTANTS_DIR/mutation_$(date +"%Y%m%d%H%M").log
touch $LOGFILE

trap "exit" INT
start_time="$(date -u +%s)"

jq -c '.[]' $MUTANTS_DIR/compile_commands.json | while read i; do
    FILE=$(echo $i | jq -r '.file')
    ARGS=$(echo $i | jq '.command')
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
    rsync -avP --no-p --no-g --remove-source-files ${orig_dir}/*.mut.*.c $MUTANTS_DIR/$path_wo_root
done

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"

echo "elapsed time: $elapsed [s]"  2>&1 | tee -a $LOGFILE
