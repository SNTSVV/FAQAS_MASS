#!/bin/bash
WDIR=/home/gsl/libutil

re='^[0-9]+$'

i=$1
gcov_file="$(basename -- $i)"
source_file="$2/${gcov_file//.gcov/}"

tst=$3

if [ -z "$tst" ];then
	matrix_file=$WDIR/tst/coverage.txt
else
	matrix_file=$tst/coverage.txt
fi
touch -a $matrix_file

function join_by { local IFS="$1"; shift; echo "$*"; }                                                                            

exist_coverage=$(cat ${matrix_file} | grep -F $source_file | wc -l)
	
if [ $exist_coverage -eq 0 ]; then
	echo -n "$source_file:" >> $matrix_file

	declare -a coverage_lines=()	

	while IFS="" read -r p || [ -n "$p" ]
	do
		read coverage line <<< $(awk -F: '{print $1, $2}' <<< "$p")
		
		if [ $line -ge 1 ]; then
			if [[ $coverage =~ $re ]] ; then
				coverage_lines+=($coverage)
			elif [ $coverage == '#####' ]; then
				coverage_lines+=("0")
			elif [ $coverage == '-'  ]; then
				coverage_lines+=("-")
			fi
		fi
	done < $i
	echo `join_by , ${coverage_lines[@]}` >> $matrix_file

else
	existing_coverage=$(cat ${matrix_file} | grep -F $source_file)
		
	# removing existing coverage line
	sed -i "\|^$source_file|d" $matrix_file	

	# getting only coverage info
	cov_existing_w_commas="${existing_coverage//$source_file\:/}"
	IFS=',' read -r -a cov_existing <<< $cov_existing_w_commas

	echo -n "$source_file:" >> $matrix_file
	declare -a coverage_lines=()

	
	while IFS="" read -r p || [ -n "$p" ]
	do
		read coverage line <<< $(awk -F: '{print $1, $2}' <<< "$p")
		
		index=$((line-1))
		if [ $line -ge 1 ]; then

			if [[ $coverage =~ $re ]]; then
				existing_value="${cov_existing[$index]}"
#				echo index $index coverage $coverage exist $existing_value
				if [ $existing_value == '-' ];then
					coverage_lines+=($coverage)
				else
					coverage_lines+=($((coverage+existing_value)))
				fi
			elif [[ "$coverage" == '#####' ]];then 
				coverage_lines+=("${cov_existing[$index]}")
			elif [ $coverage == '-'  ]; then
				coverage_lines+=("-")
			fi
		fi
	done < $i

	echo `join_by , ${coverage_lines[@]}` >> $matrix_file
fi
