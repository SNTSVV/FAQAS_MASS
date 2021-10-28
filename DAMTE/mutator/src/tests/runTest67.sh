#/bin/bash

TESTFOLDER=tests/test67/
curTest="test67"
FAULTMODEL=tests/FaultModels/faultModel67.csv
TYPE="double"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
