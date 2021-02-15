#/bin/bash

TESTFOLDER=tests/test23/
curTest="test23"
FAULTMODEL=faultModel23.csv
TYPE="int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
