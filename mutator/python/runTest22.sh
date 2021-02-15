#/bin/bash

TESTFOLDER=tests/test22/
curTest="test22"
FAULTMODEL=faultModel22.csv
TYPE="int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
