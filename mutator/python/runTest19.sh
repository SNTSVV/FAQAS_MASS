#/bin/bash

TESTFOLDER=tests/test19/
curTest="test19"
FAULTMODEL=faultModel19.csv
TYPE=float

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
