#/bin/bash

TESTFOLDER=tests/test30/
curTest="test30"
FAULTMODEL=tests/FaultModels/faultModel30.csv
TYPE="int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
