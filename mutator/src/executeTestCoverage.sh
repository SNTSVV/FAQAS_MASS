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
cp FMcoverage.py $TESTFOLDER


pushd $TESTFOLDER

export FAQAS_COVERAGE_FILE="./faqas_coverage.txt"

outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
instrumentedCompilerOutFile=${curTest}Instrumented.compile.out
valgrindOutFile=${curTest}.valgrind.out
testResults=${curTest}.results.out
gcovLog=${curTest}.gcov.out
memoryErrors=0

echo ""
echo "REMOVING PREVIOUS RESULTS..."
echo ""

rm -f $outFile
rm -f $compilerOutFile
rm -f $valgrindOutFile
rm -f $instrumentedCompilerOutFile
rm -f $testResults
rm -f $gcovLog
rm -f $FAQAS_COVERAGE_FILE

echo ""
echo "DONE"
echo ""

operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo "MAX ID: ${operations}"
echo ""

if [[ -z $_FAQAS_SINGLETON_FM ]]; then
	extra=""
	memorymode="NORMAL"
else
	extra="-D_FAQAS_SINGLETON_FM"
	memorymode="SINGLETON"
fi

x=-2
while [ $x -le $operations ]; do

  echo "OPERATION ${x} COMPILING..."

  if [ $x -eq -2 ]; then

    y=$((x-1))
    g++ $extra -Wall -fprofile-arcs -ftest-coverage -DMUTATIONOPT=$y ${curTest}.c -o main_$x >> $instrumentedCompilerOutFile 2>&1
    echo "OPERATION ${x} RUNNING..."
    ./main_$x >> $outFile 2>&1
		echo "=====" >> $outFile 2>&1
    gcov ${curTest}.c >> $gcovLog 2>&1

  else

    g++ $extra -DMUTATIONOPT=$x ${curTest}.c -std=c++11 -g -o main_$x >> $compilerOutFile 2>&1

    echo "OPERATION ${x} RUNNING..."

    valgrind --tool=memcheck --leak-check=full --track-origins=yes  --error-exitcode=3 ./main_$x >> $valgrindOutFile 2>&1

    if [ $? -eq 3 ]; then
        memoryErrors=$((memoryErrors+1))
    fi

    ./main_$x >> $outFile 2>&1
    echo "=====" >> $outFile 2>&1

  fi

    x=$((x+1))

done
echo ""
echo "DONE"

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

echo "SEE $valgrindOutFile FOR DETAILS ON MEMORY ERRORS"

echo "*************************************************************************"

python FMcoverage.py "${curTest}"

echo ""
echo "*************************************************************************"

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
echo ""

echo "${curTest},${status},${coverage},$memoryErrors MUTANTS PRESENT MEMORY ERRORS,$memorymode" >> $testResults 2>&1

popd
