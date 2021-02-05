#/bin/bash

TESTFOLDER=tests/test9/
curTest="test9"
FAULTMODEL=faultModel9.csv
TYPE=float


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
