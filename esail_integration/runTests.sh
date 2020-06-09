#!/bin/bash

SVF=/home/svf/Svf
ESAIL_FOLDER=$SVF/Models/ADCS/src/Adcs
ESAIL_FILE_TO_MUTATE=AdcsIf.cpp

SVF_MUTANTS=/home/svf/svf_mutants
mkdir -p $SVF_MUTANTS

python generateDataMutator.py 'unsigned char' faultModel.csv
mv FAQAS_dataDrivenMutator.h $ESAIL_FOLDER

pushd $SVF

for x in -2 -1 0 1 2 3 4; do
    export MUTATIONOPT=$x
    
    make install-debug
    
    if [ $? -eq 0 ]; then
        echo $x compilation PASSED 
        
        cp ./build-debug/Models/ADCS/libADCS.a $SVF_MUTANTS/libADCS.a.$x

    else
        echo $x compilation FAILED;
    fi
done

popd
