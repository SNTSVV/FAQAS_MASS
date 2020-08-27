#!/bin/bash

PROJ=/home/mlfs/mlfs/
TST=/home/mlfs/blts_workspace

PARSER=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/mlfs/update_coverage.sh
TYPE=1 # means global

shopt -s extglob

for tst in $TST/*/; do
	cd $tst/Reports/Coverage/Data	
	coverage_data=`pwd`

	for da in `find . -name '*.gcda'`; do
		output=`gcov $da`
		# do not consider gcov files with 0.00% coverage
        not_executed=`echo $output | grep 'Lines executed:0.00%' | wc -l`

		if [ $not_executed -eq 1 ]; then
	    	gcov_filename=`echo $output |  awk -F"'" '{print $4}'`
			rm $gcov_filename
	   	else
			test_name=`echo $tst | sed 's/\.//g; s:\/::g;'`
                        
			for g in `find . -name '*.gcov'`; do
				first_line=`head -n 1 $g`
				full_path=`echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath {}`
				
				path=`echo $full_path | sed 's/.*Data\///'`

			if [ $TYPE -eq 1 ];then	
				source $PARSER $g $path $coverage_data $TST
			else
				source $PARSER $g $path $coverage_data
			fi
				rm $g
			done
		fi
	done
	
	cd ../../../../
done
