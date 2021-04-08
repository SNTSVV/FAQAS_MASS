#/bin/bash

TESTFOLDER=tests/test42/
curTest="test42"
FAULTMODEL=tests/FaultModels/faultModel42.csv
TYPE=int

pushd ..
bash -x executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
