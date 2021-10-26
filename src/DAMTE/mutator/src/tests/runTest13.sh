#/bin/bash

TESTFOLDER=tests/test13/
curTest="test13"
FAULTMODEL=tests/FaultModels/faultModel13.csv
TYPE=double

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
