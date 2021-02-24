#/bin/bash

TESTFOLDER=tests/test6/
curTest="test6"
FAULTMODEL=tests/FaultModels/faultModel6.csv
TYPE="int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
