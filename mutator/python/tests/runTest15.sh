#/bin/bash

TESTFOLDER=tests/test15/
curTest="test15"
FAULTMODEL=tests/FaultModels/faultModel15.csv
TYPE="long long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
