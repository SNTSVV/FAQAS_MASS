#/bin/bash

TESTFOLDER=tests/test58/
curTest="test58"
FAULTMODEL=tests/FaultModels/faultModel58.csv
TYPE="float"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
