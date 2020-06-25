#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#
PARSER=/home/asn/parser.sh

GRAMMAR=/home/asn/grammar

for da in `find $GRAMMAR -name '*.gcda'`;do

	cd $GRAMMAR
	
	output=`gcov $da`
	echo $output

	#dont consider gcov files with 0 coverage
	not_executed=`echo $output | grep 'Lines executed:0.00%' | wc -l`
	
	if [ $not_executed -eq 1 ]; then
		gcov_filename=`echo $output | awk -F"'" '{print $4}'`
		`rm $gcov_filename`
	else
		test_name=`echo`
		for g in `find $GRAMMAR -name '*.gcov' | grep -v gcov/`;do
			first_line=`head -n 1 $g`
			path=`echo $first_line | sed -n -e 's/^.*Source://p'`
			echo path $path
			path_wo_extension=${path%.*}
			temp_dir_gcov=gcov/$path_wo_extension
			
			`mkdir -p $temp_dir_gcov`

			source $PARSER $g $path $GRAMMAR

			mv $g $temp_dir_gcov/`basename -- $g`
		done
	fi
done
