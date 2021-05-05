#!/bin/bash
rm -f results_NORMAL.csv
rm -f results_SINGLETON.csv
rm all_results.csv
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
    popd
done
popd
