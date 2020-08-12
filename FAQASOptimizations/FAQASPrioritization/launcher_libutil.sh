#!/bin/bash

HOME=/home/gsl
TST_FOLDER=$HOME/libutil/tst

MUTANTS_FOLDER=/opt/mutations/src-mutants

prioritize=./prioritize.sh
#prioritize=./asd.sh

strategy="s1"
method="ochiai"

#for mutant in `find $MUTANTS_FOLDER -name '*.c' | shuf -n 50`;do
for mutant in `find $MUTANTS_FOLDER -name 'string.mut.stmnt_for_320.734.delete.gs_string_get_suboption_bool.c'`;do
	echo $mutant
	source $prioritize $TST_FOLDER $MUTANTS_FOLDER $strategy $method $mutant
done

