#!/bin/bash

TST=/opt/results

SRC_FOLDER=/opt/sources

start_time="$(date -u +%s)"

for src in $(find $SRC_FOLDER -name '*.c');do
    lines=$(wc -l $src | cut -f1 -d' ')
    
    it=1
    for lineNumber in $(seq $it $lines);do
       
        echo $src $lineNumber 
        #srcName=`echo $src | sed "s:$SRC_FOLDER/::" | xargs dirname | sed 's:$:\.c:'`
        srcName=$(echo $src | sed "s:$SRC_FOLDER/::") 

        echo $srcName
        
        pts_file=$src.$lineNumber.coverage.txt
        
        for coverage in $(grep -ri --include coverage.txt "$srcName" $TST);do
            test_name=`echo $coverage | cut -d: -f1`
            
            IFS=',' read -r -a line_coverage <<< `echo $coverage | cut -d: -f3`
    
            if [[ ${line_coverage[$lineNumber-1]} > 0 ]];then 
                echo $test_name | tee -a $pts_file
            fi
        done
    done
    break
done

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"

echo elapsed: $elapsed [s]
