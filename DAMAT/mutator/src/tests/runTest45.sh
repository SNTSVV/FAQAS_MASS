#/bin/bash

TESTFOLDER=tests/test45/
curTest="test45"
FAULTMODEL=tests/FaultModels/faultModel45.csv

TYPE="long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
