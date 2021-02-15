#/bin/bash

TESTFOLDER=tests/test25/
curTest="test25"
FAULTMODEL=faultModel25.csv
TYPE=float

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
