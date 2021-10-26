#/bin/bash

TESTFOLDER=tests/test66/
curTest="test66"
FAULTMODEL=tests/FaultModels/faultModel66.csv
TYPE="int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
