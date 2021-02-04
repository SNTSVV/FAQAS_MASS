#/bin/bash

TESTFOLDER=tests/test6/
curTest="test6"
FAULTMODEL=faultModel4.csv
TYPE=double

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
