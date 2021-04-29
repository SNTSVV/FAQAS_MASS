#/bin/bash

TESTFOLDER=tests/test3/
curTest="test3"
FAULTMODEL=tests/FaultModels/faultModel3.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
