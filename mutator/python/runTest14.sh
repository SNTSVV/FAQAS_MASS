#/bin/bash

TESTFOLDER=tests/test14/
curTest="test14"
FAULTMODEL=faultModel14.csv
TYPE=float

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
