#!/bin/bash

TST=$1
MUTANTS=$2
STRATEGY=$3
METHOD=$4

mutant=$5

SRC_PREFIX=src/

#echo $TST $MUTANTS $STRATEGY $METHOD $mutant

if [[ $METHOD == "euclidean" ]];then
    CALC=./euclidean.py
else
    CALC=./cosine.py
fi

if [[ $STRATEGY == "s1" ]];then
    dist_func=get_distance_s1
else
    dist_func=get_distance_s2
fi

re='^[0-9]+$'

function get_distance_s1() {
	testA=$1
	testB=$2

	mutantName=$3

	testATemp=./testATemp
	testBTemp=./testBTemp

	touch $testATemp $testBTemp

	coverageA=`grep "$mutantName" $testA`

	IFS=',' read -r -a line_coverage <<< $coverageA
	count=1	
	for i in "${line_coverage[@]}";do
		if [[ $i =~ $re ]] && [[ $i > 0 ]];then
			echo $mutantName:$count >> $testATemp
		fi
		count=$((count+1))
	done

	coverageB=`grep "$mutantName" $testB`

	IFS=',' read -r -a line_coverage <<< $coverageB

	count=1
	for i in "${line_coverage[@]}";do
		if [[ $i =~ $re ]] && [[ $i > 0 ]];then
			echo $mutantName:$count >> $testBTemp
		fi
		count=$((count+1))                                                                                                               
	done

	intersection=`comm -12 <(sort $testATemp) <(sort $testBTemp) | wc -l`

	if [[ $METHOD == "jaccard" ]];then
		union=`cat $testATemp $testBTemp | sort | uniq | wc -l`
		distance=`echo "1-($intersection/$union)" | bc -l`
	else
		totalA=`cat $testATemp | wc -l`
		totalB=`cat $testBTemp | wc -l`
		distance=`echo "1-($intersection/(sqrt($totalA*$totalB)))" | bc -l`
	fi

	rm $testATemp $testBTemp
	
	echo $distance
}

function get_distance_s2() {                                                                                                       
    testA=$1
    testB=$2

    mutantName=$3

    testATemp=./testATemp
    testBTemp=./testBTemp

    touch $testATemp $testBTemp

    coverageA=`grep "$mutantName" $testA`

    IFS=',' read -r -a line_coverage <<< $coverageA
    for i in "${line_coverage[@]}";do
        if [[ $i =~ $re ]];then
            echo $i >> $testATemp
        else
            echo 0 >> $testATemp
        fi
    done

    coverageB=`grep "$mutantName" $testB`

    IFS=',' read -r -a line_coverage <<< $coverageB
    for i in "${line_coverage[@]}";do
        if [[ $i =~ $re ]];then
            echo $i >> $testBTemp
        else
            echo 0 >> $testBTemp
        fi

    done

    distance=`$CALC`

    rm $testATemp $testBTemp

    echo $distance
}

start_time="$(date -u +%s)"

prioritized=()

mutantNameTemp=`echo $mutant | sed "s:$MUTANTS/::" | xargs dirname | sed 's:$:\.c:'`
mutantName=`echo $SRC_PREFIX$mutantNameTemp`
lineNumber=`echo $mutant | awk -F[.] '{print $4}'`

#echo $mutantName

coverage_array=()

mostExecutedTest=''
count=0
for coverage in `grep -ri --include coverage.txt "$mutantName" $TST`;do
	test_name=`echo $coverage | cut -d: -f1`
	
	coverage_array+=($test_name)
	
	IFS=',' read -r -a line_coverage <<< `echo $coverage | cut -d: -f3`
	
	if [[ ${line_coverage[$lineNumber-1]} > $count ]];then
		count=${line_coverage[$lineNumber-1]}
		mostExecutedTest=$test_name
	fi	
done
total_coverage=${#coverage_array[@]}
#echo original $total_coverage 
#echo ${coverage_array[*]}

# the most exercised test
prioritized+=($mostExecutedTest)
coverage_array=("${coverage_array[@]/$mostExecutedTest}")
total_coverage=$((total_coverage-1))

inf=1000000000000

 echo before $total_coverage
while [ $total_coverage -gt 0 ];do
	
	echo ------------------------- new iteration
	
	declare -A highest_distances=()
	
	for n in ${coverage_array[@]};do
		tn=''
		minimal=$inf
		for t in ${prioritized[@]};do
			distance=`$dist_func $t $n $mutantName`
			echo t $t n $n $distance
			if (( $(echo "$distance < $minimal" | bc -l ) ));then
				minimal=$distance
				tn=$n
			fi
		done
		echo min is $tn $minimal
		highest_distances[$tn]=$minimal
	done
	
	highest_distance=$(for k in "${!highest_distances[@]}";do echo $k' - '${highest_distances["$k"]};done | sort -rn -k3 | head -n 1)
	# only for printing
	for k in "${!highest_distances[@]}";do echo $k' - '${highest_distances["$k"]};done | sort -rn -k3 | head -n 1

	highest_distance_t=`echo $highest_distance | awk -F' - ' '{print $1}'`
	highest_distance_d=`echo $highest_distance | awk -F' - ' '{print $2}'`

	if (( $(echo "$highest_distance_d == 0" | bc -l ) ));then
		break
	elif (( $(echo "$highest_distance_d > 0" | bc -l) ));then
		prioritized+=($highest_distance_t)
		coverage_array=("${coverage_array[@]/$highest_distance_t}")
		total_coverage=$((total_coverage-1))
	fi

done

echo ${coverage_array[*]}
#echo ${prioritized[*]}

pts_file=$mutant.prioritized.txt
rm -f $pts_file

for pts in ${prioritized[@]};do
	echo $pts >> $pts_file
done

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"

echo elapsed: $elapsed [s]

