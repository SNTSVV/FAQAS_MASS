#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators AOR

EXPECTED_PLUS="double function() {\nint a = 4, b = 5;\nreturn a + b;\n}"
tst_plus=`diff test.mut.3.1_3_10.AOR.function.c <(echo -e $EXPECTED_PLUS) | wc -l`

EXPECTED_MINUS="double function() {\nint a = 4, b = 5;\nreturn a - b;\n}"                                                             
tst_minus=`diff test.mut.3.2_4_10.AOR.function.c <(echo -e "$EXPECTED_MINUS") | wc -l`

EXPECTED_MULT="double function() {\nint a = 4, b = 5;\nreturn a * b;\n}"                                                             
tst_mult=`diff test.mut.3.3_2_10.AOR.function.c <(echo -e "$EXPECTED_MULT") | wc -l`

EXPECTED_MOD="double function() {\nint a = 4, b = 5;\nreturn a % b;\n}"                                                             
tst_mod=`diff test.mut.3.5_1_10.AOR.function.c <(echo -e $EXPECTED_MOD) | wc -l`

if [ $tst_plus -eq 0 ];then
    echo -e "${GREEN}TEST aor_div plus PASSED$NC"
else
    echo -e "${RED}TEST aor_div plus FAILED$NC"
fi

if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST aor_div minus PASSED$NC"
else
    echo -e "${RED}TEST aor_div minus FAILED$NC"
fi                                                                                                                                    

if [ $tst_mult -eq 0 ];then
    echo -e "${GREEN}TEST aor_div mult PASSED$NC"
else
    echo -e "${RED}TEST aor_div mult FAILED$NC"
fi                                                                                                                                    

if [ $tst_mod -eq 0 ];then
    echo -e "${GREEN}TEST aor_div mod PASSED$NC"
else
    echo -e "${RED}TEST aor_div mod FAILED$NC"
fi                                                                                                                                    

