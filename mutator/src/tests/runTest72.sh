#/bin/bash

TESTFOLDER=tests/test72/
curTest="test72"
FAULTMODEL=tests/FaultModels/faultModel72.csv
TYPE="float"
probability="20"

pushd ..
bash executeTestProbability.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE $probability
popd
