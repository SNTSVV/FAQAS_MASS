TESTFOLDER=tests/test73/
curTest="test73"
FAULTMODEL=tests/FaultModels/faultModel73.csv
TYPE="unsigned int"

pushd ..
bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
popd
