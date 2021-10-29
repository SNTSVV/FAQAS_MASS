#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

TST_NAME=$1
ORIGINAL_TIME=$2

mkdir -p $COV_FILES

TIMEOUT=$(echo "$ORIGINAL_TIME*3" | bc)

[ $TIMEOUT -eq 0 ] && TIMEOUT=3

# we save also test suite execution order
echo $TST_NAME:$TIMEOUT >> $COV_FILES/test_suite_order.txt

if [[ "$PROJ" != "$PROJ_COV" ]]; then
    pushd $PROJ_COV
    find $TST_NAME \( -name '*.gcda' -or -name '*.gcno' -or -name '*.c' -or -name '*.cpp' -or -name '*.cc' \) -exec cp {} --parents $COV_FILES \;
    popd
else
    pushd $PROJ
    find . \( -name '*.gcda' -or -name '*.gcno' -or -name '*.c' -or -name '*.cpp' -or -name '*.cc' \) -exec cp {} --parents $COV_FILES \;
    popd
fi
