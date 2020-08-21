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

EXPECTED_DIV="double function() {\nint a = 4, b = 5;\nreturn a / b;\n}"                                                             
tst_div=`diff test.mut.3.4_4_10.AOR.function.c <(echo -e $EXPECTED_DIV) | wc -l`

EXPECTED_MOD="double function() {\nint a = 4, b = 5;\nreturn a % b;\n}"                                                             
tst_mod=`diff test.mut.3.5_1_10.AOR.function.c <(echo -e $EXPECTED_MOD) | wc -l`

if [ $tst_plus -eq 0 ];then
    echo -e "${GREEN}TEST aor_mult plus PASSED$NC"
else
    echo -e "${RED}TEST aor_mult plus FAILED$NC"
fi

if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST aor_mult minus PASSED$NC"
else
    echo -e "${RED}TEST aor_mult minus FAILED$NC"
fi                                                                                                                                    

if [ $tst_div -eq 0 ];then
    echo -e "${GREEN}TEST aor_mult div PASSED$NC"
else
    echo -e "${RED}TEST aor_mult div FAILED$NC"
fi                                                                                                                                    

if [ $tst_mod -eq 0 ];then
    echo -e "${GREEN}TEST aor_mult mod PASSED$NC"
else
    echo -e "${RED}TEST aor_mult mod FAILED$NC"
fi                                                                                                                                    

