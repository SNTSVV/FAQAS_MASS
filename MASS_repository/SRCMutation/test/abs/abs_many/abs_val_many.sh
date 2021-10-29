#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test_many" --operators ABS

EXPECTED_FIRST="double function() {\ndouble a = 3;\ndouble b = 4;\nreturn -(a)+b;\n}"
EXPECTED_SND="double function() {\ndouble a = 3;\ndouble b = 4;\nreturn a+-(b);\n}"

tst_first=`diff test.mut.4.1_1_8.ABS.function.c <(echo -e $EXPECTED_FIRST) | wc -l`
tst_second=`diff test.mut.4.1_2_10.ABS.function.c <(echo -e $EXPECTED_SND) | wc -l`

if [ $tst_first -eq 0 ] && [ $tst_second -eq 0 ];then
    echo -e "${GREEN}TEST abs_val_many PASSED$NC"
else
    echo -e "${RED}TEST abs_val_many FAILED$NC"
fi
