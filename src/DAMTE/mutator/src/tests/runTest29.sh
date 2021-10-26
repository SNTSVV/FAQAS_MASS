#/bin/bash

TESTFOLDER=tests/test29/
curTest="test29"
FAULTMODEL=tests/FaultModels/faultModel29.csv
TYPE=float

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
