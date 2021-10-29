#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LCR

EXPECTED="double function() {\nint a = 4, b = 5;\nif (a > 0 && b > 0)\nreturn 1;\nelse\nreturn 0;\n}"
tst_and=`diff test.mut.3.1_1_11.LCR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_and -eq 0 ];then
    echo -e "${GREEN}TEST lcr_logic_or and PASSED$NC"
else
    echo -e "${RED}TEST lcr_logic_or and FAILED$NC"
fi                                                                                                                                    

