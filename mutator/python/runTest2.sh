#/bin/bash

TESTFOLDER=tests/test2/
curTest="test2"
FAULTMODEL=faultModel2.csv

TYPE=int

bash executeTest.sh "$TESTFOLDER" "$curTest" "$FAULTMODEL" $TYPE


