#/bin/bash

TESTFOLDER=tests/test48/
curTest="test48"
FAULTMODEL=tests/FaultModels/faultModel48.csv
TYPE="short int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
