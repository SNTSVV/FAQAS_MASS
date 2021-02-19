#/bin/bash


TESTFOLDER=$1
curTest=$2
FAULTMODEL=$3
TYPE=$4

#passes this inputs to the variables

python generateDataMutator.py "$TYPE" "$FAULTMODEL"
mv FAQAS_dataDrivenMutator.h $TESTFOLDER

#genera FAQAS_dataDrivenMutator e lo sposta nella cartella

pushd $TESTFOLDER
# si sposta in TESTFOLDER

outFile=${curTest}.out
compilerOutFile=${curTest}.compile.out
rm $outFile
#rimuove l'output precedente

pwd
#printa la current directory

operations=`grep 'MUTATIONOPT=' FAQAS_dataDrivenMutator.h | tr '/' ' ' | awk -F= '{print $2}'`
#prende mutationopt dal commento nell'header e ne ricava il numero di operazioni da svolgere
echo $operations
x=-1
while [ $x -le $operations ]; do
    g++ -DMUTATIONOPT=$x ${curTest}.c -o main_$x >> $compilerOutFile 2>&1
    #compila sostituendo MUTATIONOPT con x?
    ./main_$x >> $outFile 2>&1
    #esegue il binario
    echo "=====" >> $outFile 2>&1
    #stampa il divisorio e salva tutto in out

    x=$((x+1))
done

#compila con tutti i valori di x da -1 a operations

diff $outFile expected.out

#fa il diff

if [ $? -eq 0 ]; then
    echo PASSED
else
    echo FAILED;
fi

#verifica se il test è passato o meno

popd

#chiude la cartella
