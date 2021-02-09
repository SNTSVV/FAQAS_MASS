#/bin/bash

TESTFOLDER=tests/test10/
curTest="test10"
FAULTMODEL=faultModel10.csv
TYPE=double

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
