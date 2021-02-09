#/bin/bash

TESTFOLDER=tests/test15/
curTest="test15"
FAULTMODEL=faultModel5.csv
TYPE="long int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
