#/bin/bash

TESTFOLDER=tests/test3/
curTest="test3"
FAULTMODEL=faultModel3.csv
TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
