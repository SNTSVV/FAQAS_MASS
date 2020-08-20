#!/bin/bash

PYTHON=$1
TST=$2
STRATEGY=$3
METHOD=$4
MUTANTS_FOLDER=$5
mutant=$6
lineNumber=$7
casestudy=$8

start_time="$(date -u +%s)"

coverage_array=()

mostExecutedTest=''
count=0

mutantNameTemp=`echo $mutant | sed "s:$MUTANTS_FOLDER/::" | xargs dirname | sed 's:$:\.c:'`
#mutantNameTemp=`echo $mutant | sed "s:$MUTANTS_FOLDER/::"`                                    
mutantName=`echo $SRC_PREFIX$mutantNameTemp`

for coverage in `grep -ri --include coverage.txt "$mutantName" $TST`;do
    test_name=`echo $coverage | cut -d: -f1`
	
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

pts_file=$mutant.$lineNumber.prioritized.txt

$PYTHON /opt/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/prioritize.py --cov_array "${coverage_array[@]}" --prio "${prioritized[@]}" --mut_name "$mutantName" --line "$lineNumber" --strat "$STRATEGY" --method "$METHOD" --casestudy "$casestudy" --result "$pts_file" 

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"

echo elapsed: $elapsed [s]
