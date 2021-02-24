#/bin/bash

TESTFOLDER=tests/test7/
curTest="test7"
FAULTMODEL=tests/FaultModels/faultModel7.csv
TYPE=float

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
