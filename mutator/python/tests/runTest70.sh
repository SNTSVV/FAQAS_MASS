#/bin/bash

TESTFOLDER=tests/test70/
curTest="test70"
FAULTMODEL=tests/FaultModels/faultModel70.csv
TYPE="float"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
