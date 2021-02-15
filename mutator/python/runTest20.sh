#/bin/bash

TESTFOLDER=tests/test20/
curTest="test20"
FAULTMODEL=faultModel20.csv
TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
