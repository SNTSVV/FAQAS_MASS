#/bin/bash

TESTFOLDER=tests/test24/
curTest="test24"
FAULTMODEL=tests/FaultModels/faultModel24.csv
TYPE="short int"


pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
