#/bin/bash

TESTFOLDER=tests/test21/
curTest="test21"
FAULTMODEL=tests/FaultModels/faultModel21.csv
TYPE="short int"


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
