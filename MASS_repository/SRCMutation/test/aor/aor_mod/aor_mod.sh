#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators AOR

EXPECTED_PLUS="double function() {\nint a = 4, b = 5;\nreturn a + b;\n}"
tst_plus=`diff test.mut.3.1_2_10.AOR.function.c <(echo -e $EXPECTED_PLUS) | wc -l`

EXPECTED_MINUS="double function() {\nint a = 4, b = 5;\nreturn a - b;\n}"                                                             
tst_minus=`diff test.mut.3.2_3_10.AOR.function.c <(echo -e "$EXPECTED_MINUS") | wc -l`

EXPECTED_MULT="double function() {\nint a = 4, b = 5;\nreturn a * b;\n}"                                                             
tst_mult=`diff test.mut.3.3_1_10.AOR.function.c <(echo -e "$EXPECTED_MULT") | wc -l`

EXPECTED_DIV="double function() {\nint a = 4, b = 5;\nreturn a / b;\n}"  
tst_div=`diff test.mut.3.4_4_10.AOR.function.c <(echo -e $EXPECTED_DIV) | wc -l`

if [ $tst_plus -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod plus PASSED$NC"
else
    echo -e "${RED}TEST aor_mod plus FAILED$NC"
fi

if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod minus PASSED$NC"
else
    echo -e "${RED}TEST aor_mod minus FAILED$NC"
fi                                                                                                                                    

if [ $tst_mult -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod mult PASSED$NC"
else
    echo -e "${RED}TEST aor_mod mult FAILED$NC"
fi                                                                                                                                    

if [ $tst_div -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod div PASSED$NC"
else
    echo -e "${RED}TEST aor_mod div FAILED$NC"
fi                                                                                                                                    

