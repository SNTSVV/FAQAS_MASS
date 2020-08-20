#!/bin/bash

HOME=/home/svf
TST_FOLDER=$HOME/results

MUTANTS_FOLDER=/opt/mutations/src-mutants

PYTHON=/usr/bin/python3.6

prioritize=./prioritize.sh

strategy="s2"
method="euclidean"
casestudy="esail"

i=0
for mutant in `find $MUTANTS_FOLDER -name '*.c' | shuf -n 1`;do
# for mutant in `find $MUTANTS_FOLDER -name "$pattern"`;do   
    echo $mutant
   
    lineNumber=`echo $mutant | awk -F'.' '{print $3}'` 

    source $prioritize $PYTHON $TST_FOLDER $strategy $method $MUTANTS_FOLDER $mutant $lineNumber $casestudy
done
