#!/bin/bash

re='^[0-9]+$'

matrix_file=$1
statement_coverage=$APP_RUN_DIR/RESULTS/statement_coverage.txt

while IFS="" read -r p || [ -n "$p" ]
do
    read file coverage <<< $(awk -F':' '{print $1, $2}' <<< "$p")

    echo -n "$file:" >> $statement_coverage

    IFS=',' read -r -a cov_wo_commas <<< $coverage

    total_lines=0
    cov_lines=0
    for val in "${cov_wo_commas[@]}";do
        if [[ $val =~ $re ]]; then
            total_lines=$((total_lines+1))
            if [[ $val > 0 ]];then
                cov_lines=$((cov_lines+1))
            fi
        fi
    done
    echo -n "$cov_lines:$total_lines" >> $statement_coverage
    echo "" >> $statement_coverage

done < $matrix_file
