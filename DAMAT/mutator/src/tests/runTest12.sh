#/bin/bash

TESTFOLDER=tests/test12/
curTest="test12"
FAULTMODEL=tests/FaultModels/faultModel12.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
