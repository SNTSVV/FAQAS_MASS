#/bin/bash

TESTFOLDER=tests/test09/
curTest="test09"
FAULTMODEL=tests/FaultModels/faultModel09.csv
TYPE=float


pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
