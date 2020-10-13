#!/bin/bash

HOME=/home/csp
TST_FOLDER=$HOME/results

MUTANTS_FOLDER=/opt/mutations/src-mutants

PYTHON=/usr/bin/python3.7

prioritize=/opt/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/prioritize.sh

strategy=$1
method=$2
casestudy="param"

for mutant in `find $MUTANTS_FOLDER -name '*.c'`;do
    lines=`cat $mutant | wc -l`
    count=1

    echo $mutant

    for lineIt in `seq $count $lines`;do
        echo source $prioritize $PYTHON $TST_FOLDER $strategy $method $MUTANTS_FOLDER $mutant $lineIt $casestudy
        source $prioritize $PYTHON $TST_FOLDER $strategy $method $MUTANTS_FOLDER $mutant $lineIt $casestudy
    done
done  


