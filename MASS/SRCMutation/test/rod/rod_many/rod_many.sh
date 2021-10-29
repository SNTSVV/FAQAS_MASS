#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ROD

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a == b &&  0;\n}"
tst_a=`diff test.mut.3.1_3_18.ROD.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a == b && a ;\n}"                                                             
tst_b=`diff test.mut.3.2_4_20.ROD.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST rod_many 1 PASSED$NC"
else
    echo -e "${RED}TEST rod_many 1 FAILED$NC"
fi                                                                                                                                    

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST rod_many 2 PASSED$NC"
else
    echo -e "${RED}TEST rod_many 2 FAILED$NC"
fi                                                                                                                                    

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a  && a != 0;\n}"
tst_a=`diff test.mut.3.2_2_10.ROD.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn  b && a != 0;\n}"                                                             
tst_b=`diff test.mut.3.1_1_8.ROD.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST rod_many 3 PASSED$NC"
else
    echo -e "${RED}TEST rod_many 3 FAILED$NC"
fi                                                                                                                                    

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST rod_many 4 PASSED$NC"
else
    echo -e "${RED}TEST rod_many 4 FAILED$NC"
fi                                                                                                                                    

