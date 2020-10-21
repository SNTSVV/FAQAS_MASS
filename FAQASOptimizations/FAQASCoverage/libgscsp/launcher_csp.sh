#!/bin/bash

PROJ=/home/csp/libparam/lib/libgscsp
TST=$PROJ/tst
TYPE=0 # 1 global
DIRS_OUT="bindings|common"

PARSER=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/libgscsp/update_coverage.sh
shopt -s extglob

for d in $TST/!($DIRS_OUT)/ ; do

    cd $d/build

    for da in `find . -name '*.gcda'`; do
        output=$(gcov $da 2>&1)

        mismatch=$(echo $output | grep "stamp mismatch" | wc -l)
        if [ $mismatch -eq 1 ];then                                                                                                                                                                 
            continue
        fi 
        # do not consider gcov files with 0.00% coverage
        covered=0
        for p in $(echo $output | grep -o '[0-9.]*%');do
            percent=$(echo $p | sed 's:\%::')
            if (( $(echo "$percent > 0" | bc -l) ));then
                covered=1
            fi
        done
        
        if [ $covered -eq 0 ]; then
            gcov_filename=`echo $output |  awk -F"'" '{print $4}'`
            rm $gcov_filename
        else        
            test_name=${d//$TST/}
            
            for g in `find . -name '*.gcov'`; do
                
                first_line=`head -n 1 $g`
                path=`echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath {}`
                if [[ $path != *"$PROJ"* ]] || [[ $path == *"tst"* ]] || [[ $path == *"libutil"* ]] ; then
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
        fi
    done
    cd ../../
done         

