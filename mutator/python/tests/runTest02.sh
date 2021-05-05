#/bin/bash

TESTFOLDER=tests/test02/
curTest="test02"
FAULTMODEL=tests/FaultModels/faultModel02.csv

TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
