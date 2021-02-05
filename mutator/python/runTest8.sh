#/bin/bash

TESTFOLDER=tests/test8/
curTest="test8"
FAULTMODEL=faultModel8.csv
TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
