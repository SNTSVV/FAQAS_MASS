#/bin/bash

TESTFOLDER=tests/test56/
curTest="test56"
FAULTMODEL=tests/FaultModels/faultModel56.csv
TYPE="signed char"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
