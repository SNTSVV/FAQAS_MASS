#/bin/bash

TESTFOLDER=tests/test11/
curTest="test11"
FAULTMODEL=faultModel11.csv
TYPE=float

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
