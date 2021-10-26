#/bin/bash

TESTFOLDER=tests/test64/
curTest="test64"
FAULTMODEL=tests/FaultModels/faultModel64.csv
TYPE="long int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
