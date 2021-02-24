#/bin/bash

TESTFOLDER=tests/test18/
curTest="test18"
FAULTMODEL=tests/FaultModels/faultModel18.csv
TYPE=double

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
