#/bin/bash

TESTFOLDER=tests/test68/
curTest="test68"
FAULTMODEL=tests/FaultModels/faultModel68.csv
TYPE="long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
