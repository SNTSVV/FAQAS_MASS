#!/bin/bash

tests=`ls tests`
for x in $tests
do
echo "Cleaning $x"
pushd tests/${x}/

rm FAQAS*
rm main*
rm *.compile.out
rm *.gcov
rm *.gcda
rm *.gcno

popd
x=$((x+1))
done
