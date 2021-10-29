#/bin/bash

TESTFOLDER=tests/test69/
curTest="test69"
FAULTMODEL=tests/FaultModels/faultModel69.csv
TYPE="float"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
