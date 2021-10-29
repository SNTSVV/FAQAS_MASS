#/bin/bash

TESTFOLDER=tests/test28/
curTest="test28"
FAULTMODEL=tests/FaultModels/faultModel28.csv
TYPE=double

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
