#/bin/bash

TESTFOLDER=tests/test29/
curTest="test29"
FAULTMODEL=faultModel29.csv
TYPE=float

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
