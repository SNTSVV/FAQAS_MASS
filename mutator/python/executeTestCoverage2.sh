#/bin/bash



TESTFOLDER=$1
curTest=$2
FAULTMODEL=$3
TYPE=$4

python generateDataMutator.py "$TYPE" "$FAULTMODEL"
mv FAQAS_dataDrivenMutator.h $TESTFOLDER
cp FMcoverage2.py $TESTFOLDER


pushd $TESTFOLDER

export FAQAS_COVERAGE_FILE="./faqas_coverage.txt"

outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
instrumentedCompilerOutFile=${curTest}Instrumented.compile.out
rm $outFile

pwd

operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
echo $operations
x=-2
while [ $x -le $operations ]; do



    g++ -DMUTATIONOPT=$x ${curTest}.c -o main_$x >> $compilerOutFile 2>&1
    ./main_$x >> $outFile 2>&1
    echo "=====" >> $outFile 2>&1

    if [ $x -eq -2 ]; then
      python FMcoverage2.py "${curTest}"
    fi

    > faqas_coverage.txt

    x=$((x+1))

done



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
