#/bin/bash

TESTFOLDER=tests/test20/
curTest="test20"
FAULTMODEL=tests/FaultModels/faultModel20.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
