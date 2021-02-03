#/bin/bash

TESTFOLDER=tests/test2/
curTest="test2"
FAULTMODEL=faultModel2.csv

python generateDataMutator.py int $FAULTMODEL
mv FAQAS_dataDrivenMutator.h $TESTFOLDER



#
#	Test the fault model SIZE_IfHK
#

pushd $TESTFOLDER
outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
rm $outFile


operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo $operations
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
