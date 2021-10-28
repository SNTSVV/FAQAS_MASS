#/bin/bash

TESTFOLDER=tests/test33/
curTest="test33"
FAULTMODEL=tests/FaultModels/faultModel32.csv
TYPE="long long int"

pushd ..
bash executeTestCoverage.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
