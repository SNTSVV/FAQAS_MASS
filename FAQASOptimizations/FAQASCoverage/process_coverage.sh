#!/bin/bash

# script to be executed with argument TYPE={0,1}
# where 0 means coverage for each test, and
# 1 means overall coverage

PROJ=$1
TST=$2
TYPE=$3
DIRS_OUT=$4

PARSER=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/update_coverage.sh
shopt -s extglob

for d in $TST/!($DIRS_OUT)/ ; do
	
	cd $d/build
	
	for da in `find . -name '*.gcda'`; do
		output=`gcov $da`
		
		# do not consider gcov files with 0.00% coverage 
		not_executed=`echo $output | grep 'Lines executed:0.00%' | wc -l`

		if [ $not_executed -eq 1 ]; then 
			gcov_filename=`echo $output |  awk -F"'" '{print $4}'`
			rm $gcov_filename
		else
			test_name=${d//$TST/}
			
			for g in `find . -name '*.gcov'`; do
				
				first_line=`head -n 1 $g`
				path=`echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath {}`
					
				if [[ $path != *"$PROJ"* ]] || [[ $path == *"tst"* ]]; then
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
