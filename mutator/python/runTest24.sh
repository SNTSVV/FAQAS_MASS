#/bin/bash

TESTFOLDER=tests/test24/
curTest="test24"
FAULTMODEL=faultModel24.csv
TYPE="short int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
