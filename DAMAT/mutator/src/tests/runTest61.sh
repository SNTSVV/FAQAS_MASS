#/bin/bash

TESTFOLDER=tests/test61/
curTest="test61"
FAULTMODEL=tests/FaultModels/faultModel61.csv
TYPE="long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
