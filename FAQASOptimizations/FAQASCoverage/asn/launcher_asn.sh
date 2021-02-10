#!/bin/bash

PROJ=/home/asn/mutation_test
TST=$PROJ
TYPE=1 # 1 global
DIRS_OUT=""

PARSER=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/asn/update_coverage.sh                                      
shopt -s extglob

for da in `find $PROJ -name '*.gcda'`;do

    cd $PROJ

    output=`gcov $da`
    echo $output

    #dont consider gcov files with 0 coverage
    not_executed=`echo $output | grep 'Lines executed:0.00%' | wc -l`

    if [ $not_executed -eq 1 ]; then
        gcov_filename=`echo $output | awk -F"'" '{print $4}'`
        rm $gcov_filename
    else
        test_name=`echo`
        for g in `find $PROJ -name '*.gcov'`;do
            
			first_line=`head -n 1 $g`
            path=`echo $first_line | sed -n -e 's/^.*Source://p'`
			
			if [[ $path != *"test.c"* ]]; then
            	rm $g
                continue
            fi           
 
			path_wo_extension=${path%.*}
            temp_dir_gcov=$path_wo_extension                                                                                        

			source $PARSER $g $path $PROJ
			rm $g
        done
    fi
done

