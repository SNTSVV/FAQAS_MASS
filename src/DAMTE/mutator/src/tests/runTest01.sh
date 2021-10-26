#/bin/bash

TESTFOLDER=tests/test01/
curTest="test01"
FAULTMODEL=tests/FaultModels/faultModel01.csv
TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
