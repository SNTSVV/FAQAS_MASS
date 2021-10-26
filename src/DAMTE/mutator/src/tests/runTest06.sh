#/bin/bash

TESTFOLDER=tests/test06/
curTest="test06"
FAULTMODEL=tests/FaultModels/faultModel06.csv
TYPE="int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
