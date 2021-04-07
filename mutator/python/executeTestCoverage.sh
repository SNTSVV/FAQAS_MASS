#/bin/bash

TESTFOLDER=$1
curTest=$2
FAULTMODEL=$3
TYPE=$4

python generateDataMutator.py "$TYPE" "$FAULTMODEL"
mv FAQAS_dataDrivenMutator.h $TESTFOLDER
cp FMcoverage.py $TESTFOLDER


pushd $TESTFOLDER
outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
instrumentedCompilerOutFile=${curTest}Instrumented.compile.out
valgrindOutFile=${curTest}.valgrind.out

rm $outFile
rm $compilerOutFile
rm $valgrindOutFile
rm $instrumentedCompilerOutFile

pwd

operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo $operations
x=-2
while [ $x -le $operations ]; do

  if [ $x -eq -2 ]; then

    y=$((x-1))
    g++ -Wall -fprofile-arcs -ftest-coverage -DMUTATIONOPT=$y ${curTest}.c -o main_$x >> $instrumentedCompilerOutFile 2>&1
    echo "=====" >> $outFile 2>&1
    ./main_$x >> $outFile 2>&1
    gcov ${curTest}.c




  else
    # g++ -DMUTATIONOPT=$x ${curTest}.c -o main_$x >> $compilerOutFile 2>&1


    g++ -DMUTATIONOPT=$x ${curTest}.c -std=c++11 -g -o main_$x >> $compilerOutFile 2>&1
    valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main_$x >> $valgrindOutFile 2>&1

    ./main_$x >> $outFile 2>&1
    echo "=====" >> $outFile 2>&1

  fi

    x=$((x+1))

done

python FMcoverage.py "${curTest}"

diff FMCoverageReport_${curTest}.csv expectedCoverage.csv

if [ $? -eq 0 ]; then
    echo "COVERAGE AS EXPECTED"
else
    echo "PROBLEM WITH COVERAGE"
fi

diff $outFile expected.out

if [ $? -eq 0 ]; then
    echo PASSED
else
    echo FAILED
fi



popd
