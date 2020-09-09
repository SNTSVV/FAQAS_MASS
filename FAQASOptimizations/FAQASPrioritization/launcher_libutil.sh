#!/bin/bash

HOME=/home/gsl
TST_FOLDER=$HOME/results

MUTANTS_FOLDER=/opt/mutations/src-mutants/$1

PYTHON=/usr/bin/python3.7

prioritize=/opt/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/prioritize.sh

strategy=$2
method=$3
casestudy="gsl"

for mutant in `find $MUTANTS_FOLDER -name '*.c'`;do
    lines=`cat $mutant | wc -l`
    count=1

    echo $mutant

    #lineNumber=`echo $mutant | awk -F[.] '{print $4}'`

    for lineIt in `seq $count $lines`;do
        source $prioritize $PYTHON $TST_FOLDER $strategy $method $MUTANTS_FOLDER $mutant $lineIt $casestudy
    done
done  


