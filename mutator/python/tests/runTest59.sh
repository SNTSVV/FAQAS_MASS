#/bin/bash

TESTFOLDER=tests/test59/
curTest="test59"
FAULTMODEL=tests/FaultModels/faultModel59.csv
TYPE="double"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
