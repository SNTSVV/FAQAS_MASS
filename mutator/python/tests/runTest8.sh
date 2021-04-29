#/bin/bash

TESTFOLDER=tests/test8/
curTest="test8"
FAULTMODEL=tests/FaultModels/faultModel8.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
