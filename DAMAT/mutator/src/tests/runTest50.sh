#/bin/bash

TESTFOLDER=tests/test50/
curTest="test50"
FAULTMODEL=tests/FaultModels/faultModel50.csv
TYPE="unsigned int"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
