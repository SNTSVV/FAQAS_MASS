#/bin/bash

TESTFOLDER=tests/test32/
curTest="test32"
FAULTMODEL=tests/FaultModels/faultModel32.csv
TYPE="int"

pushd ..
bash executeTestCoverage.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
