#/bin/bash

TESTFOLDER=tests/test16/
curTest="test16"
FAULTMODEL=tests/FaultModels/faultModel16.csv
TYPE="long int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
