#!/bin/bash

PROJ=/home/csp/libparam
TST=$PROJ/tst
TYPE=0 # 1 global
DIRS_OUT="include"

PARSER=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/libparam/update_coverage.sh
shopt -s extglob

for d in $TST/!($DIRS_OUT)/ ; do

    cd $d/build

    for da in `find . -name '*.gcda'`; do
        output=`gcov $da`
        
            test_name=${d//$TST/}
            
            for g in `find . -name '*.gcov'`; do
                
                first_line=`head -n 1 $g`
                path=`echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath {}`
                if [[ $path != *"$PROJ"* ]] || [[ $path == *"tst"* ]] || [[ $path == *"libutil"* ]] || [[ $path == *"libgscsp"* ]] || [[ $path == *"libparam_client"* ]]; then
                    rm $g
                    continue
                fi

                dir_gcov_w_file=${path//$PROJ\//}
                dir_gcov_wo_file="${dir_gcov_w_file%/*}"

                if [ $TYPE -eq 0 ];then
                    source $PARSER $g $dir_gcov_wo_file $d
                else
                    source $PARSER $g $dir_gcov_wo_file
                fi
            
                rm $g
            done
    done
    cd ../../
done         

