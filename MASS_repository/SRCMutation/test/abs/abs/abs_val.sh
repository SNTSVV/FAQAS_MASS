#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ABS

EXPECTED="double function() {\ndouble a = 3;\nreturn -(a);\n}"

tst=`diff test.mut.3.1_1_8.ABS.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST abs_val PASSED$NC"
else
    echo -e "${RED}TEST abs_val FAILED$NC"
fi
