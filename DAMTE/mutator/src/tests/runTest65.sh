#/bin/bash

TESTFOLDER=tests/test65/
curTest="test65"
FAULTMODEL=tests/FaultModels/faultModel65.csv
TYPE="float"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
