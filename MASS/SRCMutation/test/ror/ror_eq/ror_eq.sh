#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ROR

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a > b;\n}"
tst_gt=`diff test.mut.3.1_4_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_gt -eq 0 ];then
    echo -e "${GREEN}TEST ror_eq gt PASSED$NC"
else
    echo -e "${RED}TEST ror_eq gt FAILED$NC"
fi                                                                                                                                    

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a >= b;\n}"
tst_ge=`diff test.mut.3.2_5_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_ge -eq 0 ];then
    echo -e "${GREEN}TEST ror_eq ge PASSED$NC"
else
    echo -e "${RED}TEST ror_eq ge FAILED$NC"                                                                                          
fi 

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a < b;\n}"
tst_lt=`diff test.mut.3.3_2_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_lt -eq 0 ];then
    echo -e "${GREEN}TEST ror_eq lt PASSED$NC"
else
    echo -e "${RED}TEST ror_eq lt FAILED$NC"                                                                                          
fi 

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a <= b;\n}"
tst_le=`diff test.mut.3.4_3_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_le -eq 0 ];then
    echo -e "${GREEN}TEST ror_eq le PASSED$NC"
else
    echo -e "${RED}TEST ror_eq le FAILED$NC"                                                                                          
fi 

EXPECTED="double function() {\nint a = 1, b = 0;\nreturn a != b;\n}"
tst_neq=`diff test.mut.3.6_1_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_neq -eq 0 ];then
    echo -e "${GREEN}TEST ror_eq neq PASSED$NC"
else
    echo -e "${RED}TEST ror_eq neq FAILED$NC"                                                                               
fi 

