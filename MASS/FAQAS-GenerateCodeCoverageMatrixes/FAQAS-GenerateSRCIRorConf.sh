#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

coverage_file=$COV_FILES/coverage.txt

srciror_coverage=$HOME/.srciror/coverage

mkdir -p "${srciror_coverage%/*}" && touch "$srciror_coverage"

function join_by { local IFS="$1"; shift; echo "$*"; }

re='^[0-9]+$'

for src in `find $PROJ_SRC \( -name '*.c' -or -name '*.cpp' -or -name '*.cc' \)`;do
    file=`basename -- $src`

    relative_file=${src//$PROJ_SRC\//}

    found=`cat $coverage_file | grep -F $relative_file | wc -l`

    if [ $found -eq 0 ];then
        echo "$file not covered; ignoring for mutation"
    else
        existing_coverage=`cat $coverage_file | grep -F $relative_file`
        cov_existing="${existing_coverage//\.\/$relative_file\:/}"

        echo -n `realpath $src`: >> $srciror_coverage
        IFS=',' read -r -a cov_array <<< $cov_existing

        line=1
        declare -a coverage_lines=()
        for i in "${cov_array[@]}";do
            if [[ $i =~ $re ]] && [[ $i > 0 ]];then
                coverage_lines+=("$line")
            fi
            line=$((line+1))
        done
        echo `join_by , ${coverage_lines[@]}` >> $srciror_coverage
    fi

done

mv $coverage_file $coverage_file.overall
