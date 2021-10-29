#/bin/bash

TESTFOLDER=tests/test49/
curTest="test49"
FAULTMODEL=tests/FaultModels/faultModel49.csv
TYPE="unsigned short int"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
