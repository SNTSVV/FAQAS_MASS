#/bin/bash

TESTFOLDER=tests/test17/
curTest="test17"
FAULTMODEL=tests/FaultModels/faultModel17.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
