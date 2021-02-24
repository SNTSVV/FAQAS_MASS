#/bin/bash

TESTFOLDER=tests/test4/
curTest="test4"
FAULTMODEL=tests/FaultModels/faultModel4.csv
TYPE=double

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
