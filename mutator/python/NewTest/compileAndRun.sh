#/bin/bash

TESTFOLDER=SUT
curTest="SUT"
FAULTMODEL=faultModel.csv
TYPE="long long int"

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
