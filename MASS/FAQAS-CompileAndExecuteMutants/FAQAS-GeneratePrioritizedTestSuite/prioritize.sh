#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

TST=$1
MUTANTS_FOLDER=$2
mutant=$3
lineNumber=$4
pts_file=$5
red_file=$6

start_time="$(date -u +%s)"

coverage_array=()

mostExecutedTest=''
count=0

mutantNameTemp=$(echo $mutant | sed "s:$MUTANTS_FOLDER/::")

mutantName=$(echo $mutantNameTemp)

for coverage in $(grep -ri --include coverage.txt "$mutantName" $TST);do
    test_name=$(echo $coverage | cut -d: -f1)
    
    IFS=',' read -r -a line_coverage <<< `echo $coverage | cut -d: -f3`
    
    if [[ ${line_coverage[$lineNumber-1]} > 0 ]];then 
        coverage_array+=($test_name)
        if [ ${line_coverage[$lineNumber-1]} -gt $count ];then
            count=${line_coverage[$lineNumber-1]}
            mostExecutedTest=$test_name
        fi  
    fi
done

prioritized=()
prioritized+=($mostExecutedTest)

coverage_array=("${coverage_array[@]}")

echo prioritize.py --cov_array "${coverage_array[@]}" --prio "${prioritized[@]}" --mut_name "$mutantName" --line "$lineNumber" --strat "s2" --method "cosine" --result "$pts_file" 
$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-GeneratePrioritizedTestSuite/prioritize.py --cov_array "${coverage_array[@]}" --prio "${prioritized[@]}" --mut_name "$mutantName" --line "$lineNumber" --strat "s2" --method "cosine" --result "$red_file" 
$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-GeneratePrioritizedTestSuite/prioritize_only.py --cov_array "${coverage_array[@]}" --prio "${prioritized[@]}" --mut_name "$mutantName" --line "$lineNumber" --strat "s2" --method "cosine" --result "$pts_file" 

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"

echo elapsed: $elapsed [s]
