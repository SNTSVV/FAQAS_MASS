#/bin/bash

TESTFOLDER=tests/test05/
curTest="test05"
FAULTMODEL=tests/FaultModels/faultModel05.csv
TYPE=double


pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
