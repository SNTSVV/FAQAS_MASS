#/bin/bash

TESTFOLDER=tests/test57/
curTest="test57"
FAULTMODEL=tests/FaultModels/faultModel57.csv
TYPE="unsigned char"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
