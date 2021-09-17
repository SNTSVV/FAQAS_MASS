#/bin/bash

TESTFOLDER=tests/test04/
curTest="test04"
FAULTMODEL=tests/FaultModels/faultModel04.csv
TYPE=double

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
