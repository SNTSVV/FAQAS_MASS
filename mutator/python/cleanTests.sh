#!/bin/bash

pushd tests
for d in */ ; do
    pushd $d
    echo "cleaning $d"
    rm -r -f DAMAt*
    rm -r -f main*
    rm -r -f *.compile.out
    rm -r -f *.gcov
    rm -r -f *.gcda
    rm -r -f *.gcno
    rm -r -f *.results.out
    rm -r -f *.valgrind.out
    rm -r -f test*.out
    rm -f -f faqas_coverage.txt
    popd
done
popd
