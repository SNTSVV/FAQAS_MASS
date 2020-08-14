#!/bin/bash

PYTHON_DIR=$1
PYTHON=$2
TST=$3
MUTANTS=$4
STRATEGY=$5
METHOD=$6
mutant=$7
casestudy=$8

SRC_PREFIX=

re='^[0-9]+$'

mutantNameTemp=`echo $mutant | sed "s:$MUTANTS/::" | xargs dirname | sed 's:$:\.c:'`
mutantName=`echo $SRC_PREFIX$mutantNameTemp`

for iter in {211..211};do

#lineNumber=`echo $mutant | awk -F[.] '{print $4}'`
lineNumber=$iter

start_time="$(date -u +%s)"

coverage_array=()

mostExecutedTest=''
count=0
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

#pts_file=$mutant.prioritized.txt
pts_file=/home/svf/simulations/$mutantName.$lineNumber.prioritized.txt

$PYTHON ./prioritize.py --cov_array "${coverage_array[@]}" --prio "${prioritized[@]}" --mut_name "$mutantName" --line "$lineNumber" --strat "$STRATEGY" --method "$METHOD" --casestudy "$casestudy" --result "$pts_file" 

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"

echo elapsed: $elapsed [s]
done
