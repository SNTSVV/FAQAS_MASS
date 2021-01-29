#!/bin/bash

SRC_MUTANTS=$1
NOT_COMPILED=$2

while read -r not_compiled_mutant; do
    not_compiled_mutant_location=$(echo $not_compiled_mutant | awk -F'|' '{print $1}')	
    list=`find $SRC_MUTANTS -name "${not_compiled_mutant_location}.c" | wc -l`
	if [ $list -gt 1 ];then
		echo $not_compiled_mutant
        location=$(echo $not_compiled_mutant | awk -F'|' '{print $2}')
        find $SRC_MUTANTS/$location -name "${not_compiled_mutant_location}.c" -delete
	else
		find $SRC_MUTANTS -name "${not_compiled_mutant_location}.c" -delete
	fi
done < "$NOT_COMPILED"

find $SRC_MUTANTS -name "*EQU*" -delete
