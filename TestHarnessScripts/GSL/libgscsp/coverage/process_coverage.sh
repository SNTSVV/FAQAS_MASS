#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

PROJECT=/home/csp/libparam/lib/libgscsp
GCOV=$PROJECT/gcov
TST=$PROJECT/tst

PARSER=$TST/update_coverage.sh

shopt -s extglob

#this script must be run with param 0 (do not consider empty coverage files) or 1 (consider)

CONSIDER_EMPTY=$1

for d in $TST/!(bindings|common)/ ; do

        cd $d/build

        for da in `find . -name '*.gcno'`; do
            output=`gcov $da`
                
            # do not consider gcov files with 0.00% coverage 
            not_executed=`echo $output | grep 'Lines executed:0.00%' | wc -l`
            
            if [ $not_executed -eq 1 ] && [[ $CONSIDER_EMPTY == *"0"* ]]; then
                    gcov_filename=`echo $output |  awk -F"'" '{print $4}'`
                    rm $gcov_filename
            else
                    test_name=${d//$TST/}

                    for g in `find . -name '*.gcov'`; do
            
                            first_line=`head -n 1 $g`
                            path=`echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath {}`
                            
                            if [[ $path != *"$PROJECT"* ]] || [[ $path == *"libutil"* ]] || [[ $path == *"tst"* ]]; then
                                rm $g 
                                continue                                                                            
                            fi 

                            dir_gcov_w_file=${path//$PROJECT\//}
                            dir_gcov_wo_file="${dir_gcov_w_file%/*}"

                            temp_dir_gcov=$GCOV/$dir_gcov_wo_file
                            mkdir -p $temp_dir_gcov
                            
                            source $PARSER $g $dir_gcov_wo_file
                            mv $g $temp_dir_gcov/$g.${test_name//\//}
                    done
            fi
        done
        cd ../../
done
