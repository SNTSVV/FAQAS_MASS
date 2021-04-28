#/bin/bash

TESTFOLDER=tests/test52/
curTest="test52"
FAULTMODEL=tests/FaultModels/faultModel52.csv
TYPE="long int"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
