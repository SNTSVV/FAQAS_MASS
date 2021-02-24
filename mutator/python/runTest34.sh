#/bin/bash

TESTFOLDER=tests/test34/
curTest="test34"
FAULTMODEL=faultModel32.csv
TYPE="long long int"

bash executeTestCoverage.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
