#/bin/bash

TESTFOLDER=tests/test44/
curTest="test44"
FAULTMODEL=tests/FaultModels/faultModel44.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
