#/bin/bash

TESTFOLDER=tests/test62/
curTest="test62"
FAULTMODEL=tests/FaultModels/faultModel62.csv
TYPE="int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
