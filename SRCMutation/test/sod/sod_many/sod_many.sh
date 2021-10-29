#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators SOD

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a >> b ||  a;\n}"
tst_a=`diff test.mut.3.1_3_18.SOD.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a >> b || b ;\n}"
tst_b=`diff test.mut.3.2_4_20.SOD.function.c <(echo -e "$EXPECTED") | wc -l`
                                                                                                                                      
if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST sod_many 1 PASSED$NC"
else
    echo -e "${RED}TEST sod_many 1 FAILED$NC"
fi

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST sod_many 2 PASSED$NC"
else
    echo -e "${RED}TEST sod_many 2 FAILED$NC"
fi

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a  || b << a;\n}"
tst_a=`diff test.mut.3.2_2_10.SOD.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn  b || b << a;\n}"
tst_b=`diff test.mut.3.1_1_8.SOD.function.c <(echo -e "$EXPECTED") | wc -l`
                                                                                                                                      
if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST sod_many 3 PASSED$NC"
else
    echo -e "${RED}TEST sod_many 3 FAILED$NC"
fi

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST sod_many 4 PASSED$NC"
else
    echo -e "${RED}TEST sod_many 4 FAILED$NC"
fi

