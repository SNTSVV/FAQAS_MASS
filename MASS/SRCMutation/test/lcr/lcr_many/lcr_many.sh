#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LCR

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a | b && 1;\n}"
tst_or=`diff test.mut.3.7_2_10.LCR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a ^ b && 1;\n}"
tst_xor=`diff test.mut.3.8_1_10.LCR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a & b || 1;\n}"
tst_and_snd=`diff test.mut.3.2_3_14.LCR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_and_snd -eq 0 ];then
    echo -e "${GREEN}TEST lcr_and and second PASSED$NC"
else
    echo -e "${RED}TEST lcr_and and second FAILED$NC"
fi                                                                                                                                    

if [ $tst_or -eq 0 ];then
    echo -e "${GREEN}TEST lcr_and or PASSED$NC"
else
    echo -e "${RED}TEST lcr_and or FAILED$NC"
fi                                                                                                                                    

if [ $tst_xor -eq 0 ];then
    echo -e "${GREEN}TEST lcr_and xor PASSED$NC"
else
    echo -e "${RED}TEST lcr_and xor FAILED$NC"
fi                                                                                                                                    
