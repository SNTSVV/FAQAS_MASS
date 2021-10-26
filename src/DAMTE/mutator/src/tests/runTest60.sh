#/bin/bash

TESTFOLDER=tests/test60/
curTest="test60"
FAULTMODEL=tests/FaultModels/faultModel60.csv
TYPE="long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
