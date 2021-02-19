#/bin/bash

TESTFOLDER=tests/test31/
curTest="test31"
FAULTMODEL=faultModel31.csv
TYPE="float"

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
