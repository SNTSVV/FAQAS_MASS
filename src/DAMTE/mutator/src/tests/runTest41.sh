#/bin/bash

TESTFOLDER=tests/test41/
curTest="test41"
FAULTMODEL=tests/FaultModels/faultModel41.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
