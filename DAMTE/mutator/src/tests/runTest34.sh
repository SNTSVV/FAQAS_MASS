#/bin/bash

TESTFOLDER=tests/test34/
curTest="test34"
FAULTMODEL=tests/FaultModels/faultModel32.csv
TYPE="long long int"

pushd ..
bash executeTestCoverage.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
