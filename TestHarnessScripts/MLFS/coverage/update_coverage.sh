#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#


matrix_file=$3/coverage.txt
touch -a $matrix_file

re='^[0-9]+$'

i=$1
gcov_file="$(basename -- $i)"
source_file=$2

exist_coverage=$(cat ${matrix_file} | grep $source_file | wc -l)
	
if [ $exist_coverage -eq 0 ]; then
	echo -n "$source_file:" >> $matrix_file

	while IFS="" read -r p || [ -n "$p" ]
	do
		read coverage line <<< $(awk -F: '{print $1, $2}' <<< "$p")
		if [ $line -ge 1 ]; then
			if [[ $coverage =~ $re ]] ; then
				echo -n "1" >> $matrix_file
			elif [ $coverage == '#####' ]; then
				echo -n "0" >> $matrix_file
			elif [ $coverage == '-'  ]; then
				echo -n "-" >> $matrix_file
			fi
		fi
	done < $i
	echo "" >> $matrix_file

else
	existing_coverage=$(cat ${matrix_file} | grep $source_file)
	
	# removing existing coverage line
	sed -i "\:$source_file:d" $matrix_file	
	# getting only coverage info

	cov_existing="${existing_coverage//$source_file\:/}"

	echo -n "$source_file:" >> $matrix_file

	while IFS="" read -r p || [ -n "$p" ]
	do
		read coverage line <<< $(awk -F: '{print $1, $2}' <<< "$p")
		index=$((line-1))

		if [ $line -ge 1 ]; then
			if [[ $coverage =~ $re ]]; then
				echo -n "1" >> $matrix_file
			elif [ "$coverage" == '#####' -a "${cov_existing:$index:1}" == '0' ]; then
				echo -n "0" >> $matrix_file
			elif [ "$coverage" == '#####' -a "${cov_existing:$index:1}" == '1' ]; then
				echo -n "1" >> $matrix_file
			elif [ $coverage == '-'  ]; then
				echo -n "-" >> $matrix_file
			fi
		fi
	done < $i
	echo "" >> $matrix_file
fi
