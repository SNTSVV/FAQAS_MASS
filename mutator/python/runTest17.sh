#/bin/bash

TESTFOLDER=tests/test17/
curTest="test17"
FAULTMODEL=faultModel17.csv
TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
