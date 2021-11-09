#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

echo 'standard probe test'
bash DAMAt_probe_insertion.sh "./test_files"  "test.c" standard "mutator.h"

diff ./test_files/expected_standard ./test_files/test.c

if [ $? = 0 ]; then
  echo "probe insertion: standard template test PASSED"
else
  echo "probe insertion: standard template test FAILED"
fi

bash DAMAt_probe_removal.sh "./test_files"  "test.c"

diff ./test_files/probe_free ./test_files/test.c

if [ $? = 0 ]; then
  echo "probe removal: standard template test PASSED"
else
  echo "probe removal: standard template test PASSED"
fi


echo 'custom probe test'
bash DAMAt_probe_insertion.sh "./test_files"  "test.c" "./test_files/custom_template" "mutator.h"

diff ./test_files/expected_custom ./test_files/test.c

if [ $? = 0 ]; then
  echo "probe insertion: custom template test PASSED"
else
  echo "probe insertion: standard template test FAILED"
fi

bash DAMAt_probe_removal.sh "./test_files"  "test.c"

diff ./test_files/probe_free ./test_files/test.c

if [ $? = 0 ]; then
  echo "probe removal: custom template test PASSED"
else
  echo "probe removal: custom template test FAILED"
fi
