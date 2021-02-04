#/bin/bash

TESTFOLDER=tests/test1/
curTest="test1"
FAULTMODEL=faultModel1.csv
TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE


