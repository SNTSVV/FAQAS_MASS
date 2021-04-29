#/bin/bash

TESTFOLDER=tests/test9/
curTest="test9"
FAULTMODEL=tests/FaultModels/faultModel9.csv
TYPE=float


pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
