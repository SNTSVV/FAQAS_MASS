#/bin/bash

TESTFOLDER=tests/test22/
curTest="test22"
FAULTMODEL=tests/FaultModels/faultModel22.csv
TYPE="short int"


pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
