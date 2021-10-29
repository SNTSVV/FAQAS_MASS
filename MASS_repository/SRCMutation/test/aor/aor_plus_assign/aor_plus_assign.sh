#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators AOR

EXPECTED_MINUS="double function() {\nint a = 4, c;\nc -= a;\nreturn c;\n}"
tst_minus=`diff test.mut.3.7_3_3.AOR.function.c <(echo -e $EXPECTED_MINUS) | wc -l`

EXPECTED_MULT="double function() {\nint a = 4, c;\nc *= a;\nreturn c;\n}"                                                             
tst_mult=`diff test.mut.3.8_2_3.AOR.function.c <(echo -e "$EXPECTED_MULT") | wc -l`

EXPECTED_DIV="double function() {\nint a = 4, c;\nc /= a;\nreturn c;\n}"                                                             
tst_div=`diff test.mut.3.9_4_3.AOR.function.c <(echo -e $EXPECTED_DIV) | wc -l`

EXPECTED_MOD="double function() {\nint a = 4, c;\nc %= a;\nreturn c;\n}"                                                             
tst_mod=`diff test.mut.3.10_1_3.AOR.function.c <(echo -e $EXPECTED_MOD) | wc -l`

if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST aor_plus_assign minus PASSED$NC"
else
    echo -e "${RED}TEST aor_plus_assign minus FAILED$NC"
fi

if [ $tst_mult -eq 0 ];then
    echo -e "${GREEN}TEST aor_plus_assign mult PASSED$NC"
else
    echo -e "${RED}TEST aor_plus_assign mult FAILED$NC"
fi                                                                                                                                    

if [ $tst_div -eq 0 ];then
    echo -e "${GREEN}TEST aor_plus_assign div PASSED$NC"
else
    echo -e "${RED}TEST aor_plus_assign div FAILED$NC"
fi                                                                                                                                    

if [ $tst_mod -eq 0 ];then
    echo -e "${GREEN}TEST aor_plus_assign mod PASSED$NC"
else
    echo -e "${RED}TEST aor_plus_assign mod FAILED$NC"
fi                                                                                                                                    

