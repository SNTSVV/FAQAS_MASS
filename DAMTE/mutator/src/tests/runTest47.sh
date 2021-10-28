#/bin/bash

TESTFOLDER=tests/test47/
curTest="test47"
FAULTMODEL=tests/FaultModels/faultModel47.csv
TYPE="long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
