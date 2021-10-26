
#/bin/bash

TESTFOLDER=tests/test25/
curTest="test25"
FAULTMODEL=tests/FaultModels/faultModel25.csv
TYPE=float

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
