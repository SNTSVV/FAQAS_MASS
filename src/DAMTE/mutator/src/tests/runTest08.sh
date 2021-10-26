#/bin/bash

TESTFOLDER=tests/test08/
curTest="test08"
FAULTMODEL=tests/FaultModels/faultModel08.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
