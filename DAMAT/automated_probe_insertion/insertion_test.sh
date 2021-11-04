#!/bin/bash
echo 'standard probe'
bash -x DAMAt_probe_insertion.sh "./test_files"  "test.c" standard

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
bash -x DAMAt_probe_insertion.sh "./test_files"  "test.c" "./test_files/custom_template"

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
