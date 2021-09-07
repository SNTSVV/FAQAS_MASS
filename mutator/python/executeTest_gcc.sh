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

mv DAMAt_dataDrivenMutator.h $TESTFOLDER


pushd $TESTFOLDER

export DAMAt_COVERAGE_FILE="./faqas_coverage.txt"

outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
valgrindOutFile=${curTest}.valgrind.out
testResults=${curTest}.results.out
memoryErrors=0

echo ""
echo "REMOVING PREVIOUS RESULTS..."

rm -f $outFile
rm -f $compilerOutFile
rm -f $valgrindOutFile
rm -f $testResults
rm -f $DAMAt_COVERAGE_FILE

echo "DONE"
echo ""

operations=`grep 'MUTATIONOPT=' DAMAt_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo "MAX ID: ${operations}"
echo ""

if [[ -z $_DAMAt_SINGLETON_FM ]]; then
	extra=""
	memorymode="NORMAL"
else
	extra="-D_DAMAt_SINGLETON_FM"
	memorymode="SINGLETON"
fi

x=-1
while [ $x -le $operations ]; do
    # gcc -DMUTATIONOPT=$x ${curTest}.c -o main_$x >> $compilerOutFile 2>&1

    echo "OPERATION ${x} COMPILING..."

    gcc -g $extra -DMUTATIONOPT=$x ${curTest}.c  -o main_$x -lm >> $compilerOutFile 2>&1
    echo "OPERATION ${x} RUNNING..."

    valgrind --tool=memcheck --leak-check=full --track-origins=yes  --error-exitcode=3 ./main_$x >> $valgrindOutFile 2>&1

    if [ $? -eq 3 ]; then

        memoryErrors=$((memoryErrors+1))

    fi

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
    echo "${curTest},PASSED,COVERAGE NOT MEASURED, $memoryErrors MUTANTS PRESENT MEMORY ERRORS,$memorymode" >> $testResults 2>&1
    echo  "${curTest} PASSED, ($memoryErrors MUTANT(S) PRESENT MEMORY ERRORS)"
else
    echo "${curTest},FAILED,COVERAGE NOT MEASURED, $memoryErrors MUTANTS PRESENT MEMORY ERRORS,$memorymode" >> $testResults 2>&1
    echo  "${curTest} FAILED, $memoryErrors MUTANT(S) PRESENT MEMORY ERRORS";
fi

echo "SEE $valgrindOutFile FOR DETAILS ON MEMORY ERRORS"

echo "*************************************************************************"
echo ""

popd
