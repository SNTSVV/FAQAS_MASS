#!/bin/bash

LIBCSP=/home/csp/libparam/lib/libgscsp
LIBCSP_FILE_TO_MUTATE=csp_io.c

CSP_MUTANTS=/home/csp/csp_mutants
mkdir -p $CSP_MUTANTS

python generateDataMutator.py 'unsigned int' faultModel.csv
mv FAQAS_dataDrivenMutator.h $LIBCSP/lib/libcsp/src

pushd $LIBCSP

for x in -2 -1 0 1 2 3; do
    ./waf --mutation-opt $x configure build
done

popd
