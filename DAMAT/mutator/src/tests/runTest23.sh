#/bin/bash

TESTFOLDER=tests/test23/
curTest="test23"
FAULTMODEL=tests/FaultModels/faultModel23.csv
TYPE="int"


pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
