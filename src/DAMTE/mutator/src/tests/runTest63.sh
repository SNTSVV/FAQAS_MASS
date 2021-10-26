#/bin/bash

TESTFOLDER=tests/test63/
curTest="test63"
FAULTMODEL=tests/FaultModels/faultModel63.csv
TYPE="double"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
