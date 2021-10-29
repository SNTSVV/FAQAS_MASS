#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators UOI

EXPECTED="void function() {\nint a = 0, b = 1;\na = (--b);\n}"
tst=`diff test.mut.3.1_2_5.UOI.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST uoi --v PASSED$NC"
else
    echo -e "${RED}TEST uoi --v FAILED$NC"
fi


EXPECTED="void function() {\nint a = 0, b = 1;\na = (b--);\n}"
tst=`diff test.mut.3.2_4_5.UOI.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST uoi v-- PASSED$NC"
else
    echo -e "${RED}TEST uoi v-- FAILED$NC"
fi
EXPECTED="void function() {\nint a = 0, b = 1;\na = (++b);\n}"
tst=`diff test.mut.3.3_1_5.UOI.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST uoi ++v PASSED$NC"
else
    echo -e "${RED}TEST uoi ++v FAILED$NC"
fi

EXPECTED="void function() {\nint a = 0, b = 1;\na = (b++);\n}"
tst=`diff test.mut.3.4_3_5.UOI.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST uoi v++ PASSED$NC"
else
    echo -e "${RED}TEST uoi v++ FAILED$NC"
fi
