#/bin/bash

TESTFOLDER=tests/test46/
curTest="test46"
FAULTMODEL=tests/FaultModels/faultModel46.csv

TYPE="long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
