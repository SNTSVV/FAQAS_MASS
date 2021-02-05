#/bin/bash

TESTFOLDER=tests/test7/
curTest="test7"
FAULTMODEL=faultModel7.csv
TYPE=float

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
