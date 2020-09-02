#!/bin/bash

SRC_MUTANTS=$1
NOT_COMPILED=$2

while read -r not_compiled_mutant; do
	find $SRC_MUTANTS -name "${not_compiled_mutant}.c" -delete
done < "$NOT_COMPILED"

find $SRC_MUTANTS -name "*EQU*" -delete
