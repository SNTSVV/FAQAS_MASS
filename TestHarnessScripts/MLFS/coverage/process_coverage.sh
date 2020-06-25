#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

PARSER=/home/mlfs/reports/update_coverage.sh

for tst in ./*/; do
	echo $tst

	cd $tst/Reports/Coverage/Data	
	coverage_data=`pwd`

	for da in `find . -name '*.gcda'`; do
		output=`gcov $da`
		# do not consider gcov files with 0.00% coverage
                not_executed=`echo $output | grep 'Lines executed:0.00%' | wc -l`

		if [ $not_executed -eq 1 ]; then
	                gcov_filename=`echo $output |  awk -F"'" '{print $4}'`
			`rm $gcov_filename`
	        else
			test_name=`echo $tst | sed 's/\.//g; s:\/::g;'`
                        
			for g in `find . -name '*.gcov'`; do
				first_line=`head -n 1 $g`
				full_path=`echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath {}`
				
				path=`echo $full_path | sed 's/.*Data\///'`
                                temp_dir_gcov=gcov/$path

				`mkdir -p $temp_dir_gcov`

				source $PARSER $g $path $coverage_data
				mv $g $temp_dir_gcov/$g.$test_name					
			done
		fi
	done
	
	cd ../../../../
done
