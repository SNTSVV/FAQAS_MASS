#/bin/bash

TESTFOLDER=tests/test27/
curTest="test27"
FAULTMODEL=tests/FaultModels/faultModel27.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
