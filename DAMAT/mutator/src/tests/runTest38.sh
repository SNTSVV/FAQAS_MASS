#/bin/bash

TESTFOLDER=tests/test38/
curTest="test38"
FAULTMODEL=tests/FaultModels/faultModel38.csv

TYPE=int

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
