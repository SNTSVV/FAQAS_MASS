#/bin/bash

TESTFOLDER=tests/test_compilation/
curTest="test_compilation"
FAULTMODEL=tests/FaultModels/faultModel_compilation.csv
TYPE="long long int"

pushd ..
bash executeTest_gcc.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" "$TYPE"
popd
