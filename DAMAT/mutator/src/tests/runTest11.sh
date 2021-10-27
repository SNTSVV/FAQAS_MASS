#/bin/bash

TESTFOLDER=tests/test11/
curTest="test11"
FAULTMODEL=tests/FaultModels/faultModel11.csv
TYPE=float

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
