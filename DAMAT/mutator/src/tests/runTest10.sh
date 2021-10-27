#/bin/bash

TESTFOLDER=tests/test10/
curTest="test10"
FAULTMODEL=tests/FaultModels/faultModel10.csv
TYPE=double

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
