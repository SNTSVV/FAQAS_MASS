#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LVR

EXPECTED="double function() {\nreturn -(3.5);\n}"
tst_minus=`diff test.mut.2.2_1_8.LVR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST lvr_literal (-l) PASSED$NC"
else
    echo -e "${RED}TEST lvr_literal (-l) FAILED$NC"
fi

EXPECTED="double function() {\nreturn 0.0;\n}"
tst_zero=`diff test.mut.2.3_2_8.LVR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_zero -eq 0 ];then
    echo -e "${GREEN}TEST lvr_literal 0.0 PASSED$NC"
else
    echo -e "${RED}TEST lvr_literal 0.0 FAILED$NC"
fi
