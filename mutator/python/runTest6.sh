#/bin/bash

TESTFOLDER=tests/test6/
curTest="test6"
FAULTMODEL=faultModel6.csv
TYPE="int"

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
