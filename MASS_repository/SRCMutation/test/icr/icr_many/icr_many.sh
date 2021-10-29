#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ICR

EXPECTED="double function() {\nreturn 1 + 1;\n}"
tst_one=`diff test.mut.2.1_6_8.ICR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nreturn (-1) + 1;\n}"
tst_minus_one=`diff test.mut.2.2_1_8.ICR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nreturn 0 + 1;\n}"
tst_zero=`diff test.mut.2.3_5_8.ICR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nreturn (10 + 1) + 1;\n}"
tst_plus_one=`diff test.mut.2.4_2_8.ICR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nreturn (10 - 1) + 1;\n}"
tst_minus_one=`diff test.mut.2.5_3_8.ICR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nreturn -(10) + 1;\n}"
tst_minus=`diff test.mut.2.6_4_8.ICR.function.c <(echo -e $EXPECTED) | wc -l`


EXPECTED="double function() {\nreturn 10 + (-1);\n}"
tst_minus_snd=`diff test.mut.2.2_7_13.ICR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nreturn 10 + 0;\n}"
tst_zero_snd=`diff test.mut.2.3_8_13.ICR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nreturn 10 + 2;\n}"
tst_plus_one_snd=`diff test.mut.2.4_9_13.ICR.function.c <(echo -e $EXPECTED) | wc -l`


if [ $tst_minus_snd -eq 0 ];then
    echo -e "${GREEN}TEST icr_many -(i) second PASSED$NC"
else
    echo -e "${RED}TEST icr_many -(i) second FAILED$NC"
fi

if [ $tst_zero_snd -eq 0 ];then
    echo -e "${GREEN}TEST icr_many zero second PASSED$NC"
else
    echo -e "${RED}TEST icr_many zero second FAILED$NC"
fi

if [ $tst_plus_one_snd -eq 0 ];then
    echo -e "${GREEN}TEST icr_many +1 second PASSED$NC"
else
    echo -e "${RED}TEST icr_many +1 second FAILED$NC"
fi

if [ $tst_one -eq 0 ];then
    echo -e "${GREEN}TEST icr_many 1 PASSED$NC"
else
    echo -e "${RED}TEST icr_many 1 FAILED$NC"
fi
if [ $tst_minus_one -eq 0 ];then
    echo -e "${GREEN}TEST icr_many (-1) PASSED$NC"
else
    echo -e "${RED}TEST icr_many (-1) FAILED$NC"
fi
if [ $tst_zero -eq 0 ];then
    echo -e "${GREEN}TEST icr_many 0 PASSED$NC"
else
    echo -e "${RED}TEST icr_many 0 FAILED$NC"
fi
if [ $tst_plus_one -eq 0 ];then
    echo -e "${GREEN}TEST icr_many i + 1 PASSED$NC"
else
    echo -e "${RED}TEST icr_many i + 1 FAILED$NC"
fi
if [ $tst_minus_one -eq 0 ];then
    echo -e "${GREEN}TEST icr_many i - 1 PASSED$NC"
else
    echo -e "${RED}TEST icr_many i - 1 FAILED$NC"
fi
if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST icr_many -(i) PASSED$NC"
else
    echo -e "${RED}TEST icr_many -(i) FAILED$NC"
fi
