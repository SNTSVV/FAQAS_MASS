#/bin/bash

TESTFOLDER=tests/test5/
curTest="test5"
FAULTMODEL=faultModel5.csv
TYPE=double


bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE
