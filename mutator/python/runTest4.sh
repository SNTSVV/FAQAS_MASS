#/bin/bash

TESTFOLDER=tests/test4/
curTest="test4"
FAULTMODEL=faultModel4.csv
TYPE=double

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
