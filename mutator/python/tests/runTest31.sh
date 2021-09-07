#/bin/bash

TESTFOLDER=tests/test31/
curTest="test31"
FAULTMODEL=tests/FaultModels/faultModel31.csv
TYPE="float"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
