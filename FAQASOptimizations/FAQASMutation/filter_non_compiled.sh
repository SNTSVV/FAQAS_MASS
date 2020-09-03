#!/bin/bash

SRC_MUTANTS=$1
NOT_COMPILED=$2

while read -r not_compiled_mutant; do
	list=`find $SRC_MUTANTS -name "${not_compiled_mutant}.c" | wc -l`
	
	if [ $list -gt 1 ];then
		echo $not_compiled_mutant
	else
		find $SRC_MUTANTS -name "${not_compiled_mutant}.c" -delete
	fi
done < "$NOT_COMPILED"

find $SRC_MUTANTS -name "*EQU*" -delete
