#/bin/bash

TESTFOLDER=tests/test2/
curTest="test2"
FAULTMODEL=tests/FaultModels/faultModel2.csv

TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
