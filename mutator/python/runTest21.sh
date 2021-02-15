#/bin/bash

TESTFOLDER=tests/test21/
curTest="test21"
FAULTMODEL=faultModel21.csv
TYPE="int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
