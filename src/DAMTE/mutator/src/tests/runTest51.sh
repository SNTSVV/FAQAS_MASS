#/bin/bash

TESTFOLDER=tests/test51/
curTest="test51"
FAULTMODEL=tests/FaultModels/faultModel51.csv
TYPE="int"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
