#!/bin/bash

# this
TST_NAME=$1

mkdir -p $COV_FILES

# we save also test suite execution order
echo $TST_NAME >> $COV_FILES/test_suite_order.txt

find $TST_NAME \( -name '*.gcda' -or -name '*.gcno' \) -exec cp {} --parents $COV_FILES \;
