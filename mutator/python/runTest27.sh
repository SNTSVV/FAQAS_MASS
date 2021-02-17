#/bin/bash

TESTFOLDER=tests/test27/
curTest="test27"
FAULTMODEL=faultModel27.csv
TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
