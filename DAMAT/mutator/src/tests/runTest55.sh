#/bin/bash

TESTFOLDER=tests/test55/
curTest="test55"
FAULTMODEL=tests/FaultModels/faultModel55.csv
TYPE="unsigned long long int"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
