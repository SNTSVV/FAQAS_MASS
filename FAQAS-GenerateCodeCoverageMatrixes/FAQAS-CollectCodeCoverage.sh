#!/bin/bash

# this
COVERAGE_DIR=$1

mkdir -p $COV_FILES

find $COVERAGE_DIR \( -name '*.gcda' -or -name '*.gcno' \) -exec cp {} --parents $COV_FILES \;
