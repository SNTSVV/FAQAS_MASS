#/bin/bash

TESTFOLDER=tests/test71/
curTest="test71"
FAULTMODEL=tests/FaultModels/faultModel71.csv
TYPE="float"

pushd ..
bash executeTestJustOnce.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
