#/bin/bash

TESTFOLDER=tests/test53/
curTest="test53"
FAULTMODEL=tests/FaultModels/faultModel53.csv
TYPE="unsigned long int"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
