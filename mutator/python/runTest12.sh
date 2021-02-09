#/bin/bash

TESTFOLDER=tests/test12/
curTest="test12"
FAULTMODEL=faultModel12.csv
TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
