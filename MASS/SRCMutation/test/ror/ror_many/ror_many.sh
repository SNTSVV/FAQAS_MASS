#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ROR

EXPECTED="double function() {\nint a = 1, b = 0;\nif (!(a))\nreturn a > b;\nreturn 0;\n}"
tst_if=`diff test.mut.3.7_1_5.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_if -eq 0 ];then
    echo -e "${GREEN}TEST ror_many if PASSED$NC"
else
    echo -e "${RED}TEST ror_many if FAILED$NC"
fi

EXPECTED="double function() {\nint a = 1, b = 0;\nif (a)\nreturn a >= b;\nreturn 0;\n}"
tst_ge=`diff test.mut.4.2_5_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_ge -eq 0 ];then
    echo -e "${GREEN}TEST ror_many ge PASSED$NC"
else
    echo -e "${RED}TEST ror_many ge FAILED$NC"
fi

EXPECTED="double function() {\nint a = 1, b = 0;\nif (a)\nreturn a < b;\nreturn 0;\n}"
tst_lt=`diff test.mut.4.3_2_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_lt -eq 0 ];then
    echo -e "${GREEN}TEST ror_many lt PASSED$NC"
else
    echo -e "${RED}TEST ror_many lt FAILED$NC"
fi

EXPECTED="double function() {\nint a = 1, b = 0;\nif (a)\nreturn a <= b;\nreturn 0;\n}"
tst_le=`diff test.mut.4.4_3_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_le -eq 0 ];then
    echo -e "${GREEN}TEST ror_many le PASSED$NC"
else
    echo -e "${RED}TEST ror_many le FAILED$NC"
fi

EXPECTED="double function() {\nint a = 1, b = 0;\nif (a)\nreturn a == b;\nreturn 0;\n}"
tst_eq=`diff test.mut.4.5_4_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_eq -eq 0 ];then
    echo -e "${GREEN}TEST ror_many eq PASSED$NC"
else
    echo -e "${RED}TEST ror_many eq FAILED$NC"
fi

EXPECTED="double function() {\nint a = 1, b = 0;\nif (a)\nreturn a != b;\nreturn 0;\n}"
tst_neq=`diff test.mut.4.6_1_10.ROR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_neq -eq 0 ];then
    echo -e "${GREEN}TEST ror_many neq PASSED$NC"
else
    echo -e "${RED}TEST ror_many neq FAILED$NC"
fi
