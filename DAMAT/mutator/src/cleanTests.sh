#!/bin/bash
pushd tests
for d in */ ; do
    pushd $d
    echo "cleaning $d"
    rm -r -f FAQAS*
    rm -r -f main*
    rm -r -f *.compile.out
    rm -r -f *.gcov
    rm -r -f *.gcda
    rm -r -f *.gcno
    rm -r -f *.results.out
    rm -f -f faqas_coverage.txt
    popd
done
popd
