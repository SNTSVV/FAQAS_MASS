#!/bin/bash

pushd tests
for d in */ ; do
    pushd $d
    echo "cleaning $d"
    rm FAQAS*
    rm main*
    rm *.compile.out
    rm *.gcov
    rm *.gcda
    rm *.gcno
    popd
done
popd
