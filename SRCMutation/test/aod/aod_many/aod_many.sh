#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators AOD

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a  + 1;\n}"
tst_a=`diff test.mut.3.2_3_10.AOD.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a / b ;\n}"
tst_b=`diff test.mut.3.2_4_14.AOD.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST aod_many 1 PASSED$NC"
else
    echo -e "${RED}TEST aod_many 1 FAILED$NC"
fi

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST aod_many 2 PASSED$NC"
else
    echo -e "${RED}TEST aod_many 2 FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn  1;\n}"
tst_a=`diff test.mut.3.1_1_8.AOD.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn  b + 1;\n}"                                                             
tst_b=`diff test.mut.3.1_2_8.AOD.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST aod_many 3 PASSED$NC"
else
    echo -e "${RED}TEST aod_many 3 FAILED$NC"
fi                                                                                                                                    

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST aod_many 4 PASSED$NC"
else
    echo -e "${RED}TEST aod_many 4 FAILED$NC"
fi                                                                                                                                    

