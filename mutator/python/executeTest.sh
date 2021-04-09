#/bin/bash

TESTFOLDER=$1
curTest=$2
FAULTMODEL=$3
TYPE=$4

echo ""
echo ""
echo "*************************************************************************"
echo "*************************************************************************"
echo "RUNNING ${curTest}"
echo "*************************************************************************"
echo "*************************************************************************"
echo ""
echo ""

echo "FAULT MODEL:"
echo ""

python generateDataMutator.py "$TYPE" "$FAULTMODEL"
mv FAQAS_dataDrivenMutator.h $TESTFOLDER


pushd $TESTFOLDER
outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
valgrindOutFile=${curTest}.valgrind.out
testResults=${curTest}.results.out

echo ""
echo "REMOVING PREVIOUS RESULTS..."

rm $outFile
rm $compilerOutFile
rm $valgrindOutFile
rm $testResults

echo "DONE"
echo ""

operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo "NUMBER OF OPERATIONS: ${operations}"
echo ""

x=-1
while [ $x -le $operations ]; do
    # g++ -DMUTATIONOPT=$x ${curTest}.c -o main_$x >> $compilerOutFile 2>&1

    echo "OPERATION ${x} COMPILING..."

    g++ -DMUTATIONOPT=$x ${curTest}.c -std=c++11 -g -o main_$x >> $compilerOutFile 2>&1
    valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main_$x >> $valgrindOutFile 2>&1

    echo "OPERATION ${x} RUNNING..."

    ./main_$x >> $outFile 2>&1
    echo "=====" >> $outFile 2>&1

    x=$((x+1))
done
echo ""
echo "DONE"

echo ""
echo "*************************************************************************"

diff $outFile expected.out

if [ $? -eq 0 ]; then
    echo "${curTest},PASSED,COVERAGE NOT MEASURED" >> $testResults 2>&1
    echo  "${curTest} PASSED"
else
    echo "${curTest},FAILED,COVERAGE NOT MEASURED" >> $testResults 2>&1
    echo  "${curTest} FAILED";
fi

echo "*************************************************************************"
echo ""

popd
