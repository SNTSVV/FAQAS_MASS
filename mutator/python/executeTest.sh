#/bin/bash


TESTFOLDER=$1
curTest=$2
FAULTMODEL=$3
TYPE=$4

python generateDataMutator.py $TYPE $FAULTMODEL
mv FAQAS_dataDrivenMutator.h $TESTFOLDER


pushd $TESTFOLDER
outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
rm $outFile

pwd

operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo $operations
x=-1
while [ $x -le $operations ]; do
    g++ -DMUTATIONOPT=$x ${curTest}.c -o main_$x >> $compilerOutFile 2>&1
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
