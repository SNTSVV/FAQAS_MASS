#!/bin/bash

TST_NAME=$1
ORIGINAL_TIME=$2

mkdir -p $COV_FILES

TIMEOUT=$(echo "$ORIGINAL_TIME*3" | bc)

# we save also test suite execution order
echo $TST_NAME:$TIMEOUT >> $COV_FILES/test_suite_order.txt

find $TST_NAME \( -name '*.gcda' -or -name '*.gcno' -or -name '*.c' \) -exec cp {} --parents $COV_FILES \;
