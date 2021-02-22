#/bin/bash

TESTFOLDER=tests/test32/
curTest="SUT"
FAULTMODEL=faultModel32.csv
TYPE="long long int"

bash executeTestCoverage.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
