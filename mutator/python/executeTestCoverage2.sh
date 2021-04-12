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
cp FMcoverage2.py $TESTFOLDER


pushd $TESTFOLDER

export FAQAS_COVERAGE_FILE="./faqas_coverage.txt"

outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
valgrindOutFile=${curTest}.valgrind.out
testResults=${curTest}.results.out
memoryErrors=0

echo ""
echo "REMOVING PREVIOUS RESULTS..."

rm $outFile
rm $compilerOutFile
rm $valgrindOutFile
rm $instrumentedCompilerOutFile
rm $testResults
rm $FAQAS_COVERAGE_FILE

echo "DONE"
echo ""

operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo "MAX ID: ${operations}"
echo ""

x=-2
while [ $x -le $operations ]; do

    echo "OPERATION ${x} COMPILING..."

    g++ -DMUTATIONOPT=$x ${curTest}.c -std=c++11 -g -o main_$x >> $compilerOutFile 2>&1

    echo "OPERATION ${x} RUNNING..."

    valgrind --tool=memcheck --leak-check=full --track-origins=yes  --error-exitcode=666 ./main_$x >> $valgrindOutFile 2>&1

    if [ $? -eq 666 ]; then
        $memoryErrors=$memoryErrors+1
    fi

    rm $FAQAS_COVERAGE_FILE

    ./main_$x >> $outFile 2>&1

    echo "=====" >> $outFile 2>&1


    if [ $x -eq -2 ]; then
      python FMcoverage2.py "${curTest}"
    fi

    > faqas_coverage.txt

    x=$((x+1))

done
echo ""
echo "DONE"

echo ""

diff FMCoverageReport_${curTest}.csv expectedCoverage.csv

if [ $? -eq 0 ]; then
  echo ""
  echo "*************************************************************************"
    echo "COVERAGE AS EXPECTED"
    coverage="COVERAGE AS EXPECTED"
else
  echo ""
  echo "*************************************************************************"
    echo "PROBLEM WITH COVERAGE"
    coverage="PROBLEM WITH COVERAGE"
fi

echo "*************************************************************************"

diff $outFile expected.out

if [ $? -eq 0 ]; then
  echo ""
  echo "*************************************************************************"
    echo  "${curTest} PASSED ($memoryErrors MUTANT(S) PRESENT MEMORY ERRORS)"
    status="PASSED"
else
  echo ""
  echo "*************************************************************************"
    echo  "${curTest} FAILED ($memoryErrors MUTANT(S) PRESENT MEMORY ERRORS)"
    status="FAILED";
fi

echo "${curTest},${status},${coverage},$memoryErrors MUTANTS PRESENT MEMORY ERRORS" >> $testResults 2>&1


echo "*************************************************************************"
echo ""

popd
