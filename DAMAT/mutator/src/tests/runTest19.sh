#/bin/bash

TESTFOLDER=tests/test19/
curTest="test19"
FAULTMODEL=tests/FaultModels/faultModel19.csv
TYPE=float

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
