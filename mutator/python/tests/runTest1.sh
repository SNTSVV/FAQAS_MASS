#/bin/bash

TESTFOLDER=tests/test1/
curTest="test1"
FAULTMODEL=tests/FaultModels/faultModel1.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
