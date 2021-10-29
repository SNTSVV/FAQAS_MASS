#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ROR

EXPECTED="double function() {\nint a = 4;\nif (!(a))\nreturn 1;\nelse\nreturn 0;\n}"
tst=`diff test.mut.3.7_1_5.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST ror_if PASSED$NC"
else
    echo -e "${RED}TEST ror_if FAILED$NC"
fi                                                                                                                                    

