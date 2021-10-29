#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ROR

EXPECTED="void function() {\nint a = 4;\nwhile (!(a))\na+=1;\n}"
tst_while=`diff test.mut.3.8_1_8.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_while -eq 0 ];then
    echo -e "${GREEN}TEST ror_while PASSED$NC"
else
    echo -e "${RED}TEST ror_while FAILED$NC"
fi                                                                                                                                    

