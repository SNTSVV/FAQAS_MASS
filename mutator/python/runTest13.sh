#/bin/bash

TESTFOLDER=tests/test13/
curTest="test13"
FAULTMODEL=faultModel13.csv
TYPE=double

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
