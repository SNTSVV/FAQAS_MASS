#/bin/bash

TESTFOLDER=tests/test5/
curTest="test5"
FAULTMODEL=tests/FaultModels/faultModel5.csv
TYPE=double


pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
