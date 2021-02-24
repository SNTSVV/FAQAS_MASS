#/bin/bash

TESTFOLDER=tests/test33/
curTest="test33"
FAULTMODEL=faultModel32.csv
TYPE="long long int"

bash executeTestCoverage.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
