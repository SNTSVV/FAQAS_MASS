#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

matrix_file=overall.txt
srciror_coverage=srciror_file
touch -a $srciror_coverage

MLFS=/home/mlfs/mlfs

function join_by { local IFS="$1"; shift; echo "$*"; }

for i in `find $MLFS/libm -name "*.c"`; do
	file=`basename -- $i`
	relative_file=${i//$MLFS\//}
	found=`cat $matrix_file | grep $file | wc -l`

	if [ $found -eq 0 ]; then
		echo "$file not found"
        else
	        existing_coverage=`cat ${matrix_file} | grep $file`
		cov_existing="${existing_coverage//$relative_file\:/}"
		echo -n `realpath $i`: >> $srciror_coverage

		line=1
		declare -a coverage_lines=()

		for j in `echo $cov_existing | sed -e 's/\(.\)/\1\n/g'`;do
			if [ $j == '1' ]; then
				coverage_lines+=("$line")
			fi
			line=$((line+1))
		done

		echo `join_by , ${coverage_lines[@]}` >> $srciror_coverage
	fi
done
