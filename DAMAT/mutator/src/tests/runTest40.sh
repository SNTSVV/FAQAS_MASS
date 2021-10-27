#/bin/bash

TESTFOLDER=tests/test40/
curTest="test40"
FAULTMODEL=tests/FaultModels/faultModel40.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
