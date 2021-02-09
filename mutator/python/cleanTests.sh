#!/bin/bash

tests=`ls tests`
for x in $tests
do
echo "Cleaning $x"
pushd tests/${x}/

rm FAQAS*
rm main*
rm test*.compile.out

popd
x=$((x+1))
done
