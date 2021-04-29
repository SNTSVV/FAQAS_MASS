#/bin/bash

TESTFOLDER=tests/test36/
curTest="test36"
FAULTMODEL=tests/FaultModels/faultModel32.csv
TYPE="long long int"

pushd ..
bash executeTestCoverage2.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
