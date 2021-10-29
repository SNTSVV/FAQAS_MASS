#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators AOR

EXPECTED="double function() {\nint a = 4, c;\nc += a;\nreturn c;\n}"
tst_plus=`diff test.mut.3.6_2_3.AOR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nint a = 4, c;\nc -= a;\nreturn c;\n}"                                                             
tst_minus=`diff test.mut.3.7_3_3.AOR.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, c;\nc *= a;\nreturn c;\n}"                                                             
tst_mult=`diff test.mut.3.8_1_3.AOR.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, c;\nc /= a;\nreturn c;\n}"                                                             
tst_div=`diff test.mut.3.9_4_3.AOR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_plus -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod_assign plus PASSED$NC"
else
    echo -e "${RED}TEST aor_mod_assign plus FAILED$NC"
fi

if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod_assign minus PASSED$NC"
else
    echo -e "${RED}TEST aor_mod_assign minus FAILED$NC"
fi                                                                                                                                    

if [ $tst_mult -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod_assign mult PASSED$NC"
else
    echo -e "${RED}TEST aor_mod_assign mult FAILED$NC"
fi                                                                                                                                    

if [ $tst_div -eq 0 ];then
    echo -e "${GREEN}TEST aor_mod_assign div PASSED$NC"
else
    echo -e "${RED}TEST aor_mod_assign div FAILED$NC"
fi                                                                                                                                    

