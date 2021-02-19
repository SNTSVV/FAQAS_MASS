#/bin/bash

TESTFOLDER=tests/test30/
curTest="test30"
FAULTMODEL=faultModel30.csv
TYPE="int"

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
