#/bin/bash

TESTFOLDER=tests/test18/
curTest="test18"
FAULTMODEL=faultModel18.csv
TYPE=double

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
