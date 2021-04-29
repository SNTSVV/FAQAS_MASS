#/bin/bash

TESTFOLDER=tests/test43/
curTest="test43"
FAULTMODEL=tests/FaultModels/faultModel43.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
