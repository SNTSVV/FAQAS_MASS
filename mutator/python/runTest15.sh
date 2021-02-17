#/bin/bash

TESTFOLDER=tests/test15/
curTest="test15"
FAULTMODEL=faultModel15.csv
TYPE="long long int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
