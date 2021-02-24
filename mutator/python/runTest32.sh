#/bin/bash

TESTFOLDER=tests/test32/
curTest="test32"
FAULTMODEL=faultModel32.csv
TYPE="long long int"

bash executeTestCoverage.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
