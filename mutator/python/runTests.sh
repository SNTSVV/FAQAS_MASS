#!/bin/bash

python generateDataMutator.py int faultModel.csv
mv FAQAS_dataDrivenMutator.h tests/test1/



#
#	Test the fault model SIZE_IfHK
#

pushd tests/test1/
curTest="test1"
outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
rm $outFile


operations=`grep 'define SIZE_IfHK' FAQAS_dataDrivenMutator.h | awk '{print $3}'`

x=-1
while [ $x -lt $operations ]; do
    g++ -DMUTATIONOPT=$x test1.c -o main_$x >> $compilerOutFile 2>&1
    ./main_$x >> $outFile 2>&1
    echo "=====" >> $outFile 2>&1

    x=$((x+1))
done

diff $outFile expected.out

if [ $? -eq 0 ]; then
    echo PASSED 
else 
    echo FAILED; 
fi
popd
