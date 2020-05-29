

python generateDataMutator.py int faultModel.csv
mv FAQAS_dataDrivenMutator.h tests/test1/

pushd tests/test1/
curTest="test1"
outFile=${curTest}.out
rm $outFile
for x in -1 0 1 2 3 4; do
    g++ -DMUTATIONOPT=$x test1.c -o main_$x >> $outFile
    ./main_$x >> $outFile
    echo "=====" >> $outFile
done

diff $outFile expected.out
if [ $? -eq 0 ]; then
    echo PASSED 
else 
    echo FAILED; 
fi
popd
