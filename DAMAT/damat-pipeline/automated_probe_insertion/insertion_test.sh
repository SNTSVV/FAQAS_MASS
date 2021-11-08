#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

echo 'standard probe'
bash -x DAMAt_probe_insertion.sh "./test_files"  "test.c" standard "mutator.h"

diff ./test_files/expected_standard ./test_files/test.c

if [ $? = 0 ]; then
  echo PASSED
else
  echo FAILED
fi

bash -x DAMAt_probe_removal.sh "./test_files"  "test.c"

diff ./test_files/probe_free ./test_files/test.c

if [ $? = 0 ]; then
  echo PASSED
else
  echo FAILED
fi


echo 'custom probe'
bash -x DAMAt_probe_insertion.sh "./test_files"  "test.c" "./test_files/custom_template" "mutator.h"

diff ./test_files/expected_custom ./test_files/test.c

if [ $? = 0 ]; then
  echo PASSED
else
  echo FAILED
fi

bash -x DAMAt_probe_removal.sh "./test_files"  "test.c"

diff ./test_files/probe_free ./test_files/test.c

if [ $? = 0 ]; then
  echo PASSED
else
  echo FAILED
fi
