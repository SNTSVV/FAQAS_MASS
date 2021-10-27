#/bin/bash

TESTFOLDER=tests/test14/
curTest="test14"
FAULTMODEL=tests/FaultModels/faultModel14.csv
TYPE=float

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
