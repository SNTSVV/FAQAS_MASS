#/bin/bash

TESTFOLDER=tests/test26/
curTest="test26"
FAULTMODEL=faultModel26.csv
TYPE=double

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
