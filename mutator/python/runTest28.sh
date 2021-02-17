#/bin/bash

TESTFOLDER=tests/test28/
curTest="test28"
FAULTMODEL=faultModel28.csv
TYPE=double

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
