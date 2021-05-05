#/bin/bash

TESTFOLDER=tests/test03/
curTest="test03"
FAULTMODEL=tests/FaultModels/faultModel03.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
