#/bin/bash

TESTFOLDER=tests/test54/
curTest="test54"
FAULTMODEL=tests/FaultModels/faultModel54.csv
TYPE="long long int"

echo $TYPE

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
