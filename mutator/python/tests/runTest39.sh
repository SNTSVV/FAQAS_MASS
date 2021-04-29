#/bin/bash

TESTFOLDER=tests/test39/
curTest="test39"
FAULTMODEL=tests/FaultModels/faultModel39.csv

TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
