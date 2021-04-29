#!/bin/bash

pushd tests
for d in */ ; do
    pushd $d
    echo "cleaning $d"
    rm -f FAQAS*
    rm -f main*
    rm -f *.compile.out
    rm -f *.gcov
    rm -f *.gcda
    rm -f *.gcno
    popd
done
popd
