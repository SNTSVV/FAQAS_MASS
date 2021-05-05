#/bin/bash

TESTFOLDER=tests/test07/
curTest="test07"
FAULTMODEL=tests/FaultModels/faultModel07.csv
TYPE=float

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
