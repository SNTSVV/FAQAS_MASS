#/bin/bash

TESTFOLDER=tests/test26/
curTest="test26"
FAULTMODEL=tests/FaultModels/faultModel26.csv
TYPE=double

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
