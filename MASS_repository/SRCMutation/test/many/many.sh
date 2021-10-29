#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" 

EXPECTED_MINUS="double function() {\nint a = 4, b = 5;\nreturn a - b;\n}"
tst_minus=`diff test.mut.3.2_3_10.AOR.function.c <(echo -e $EXPECTED_MINUS) | wc -l`

EXPECTED_MULT="double function() {\nint a = 4, b = 5;\nreturn a * b;\n}"                                                             
tst_mult=`diff test.mut.3.3_2_10.AOR.function.c <(echo -e "$EXPECTED_MULT") | wc -l`

EXPECTED_DIV="double function() {\nint a = 4, b = 5;\nreturn a / b;\n}"                                                             
tst_div=`diff test.mut.3.4_4_10.AOR.function.c <(echo -e $EXPECTED_DIV) | wc -l`

EXPECTED_MOD="double function() {\nint a = 4, b = 5;\nreturn a % b;\n}"                                                             
tst_mod=`diff test.mut.3.5_1_10.AOR.function.c <(echo -e $EXPECTED_MOD) | wc -l`

if [ $tst_minus -eq 0 ];then
    echo -e "${GREEN}TEST many AOR minus PASSED$NC"
else
    echo -e "${RED}TEST many AOR minus FAILED$NC"
fi

if [ $tst_mult -eq 0 ];then
    echo -e "${GREEN}TEST many AOR mult PASSED$NC"
else
    echo -e "${RED}TEST many AOR mult FAILED$NC"
fi                                                                                                                                    

if [ $tst_div -eq 0 ];then
    echo -e "${GREEN}TEST many AOR div PASSED$NC"
else
    echo -e "${RED}TEST many AOR div FAILED$NC"
fi                                                                                                                                    

if [ $tst_mod -eq 0 ];then
    echo -e "${GREEN}TEST many AOR mod PASSED$NC"
else
    echo -e "${RED}TEST many AOR mod FAILED$NC"
fi                                                                                                                                   

EXPECTED="double function() {\nint a = 4, b = 1;\nreturn a + b;\n}"
tst=`diff test.mut.2.1_12_16.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\n\nreturn a + b;\n}"
tst=`diff test.mut.2.1_1_1.SDL.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many SDL PASSED$NC"
else
    echo -e "${RED}TEST many SDL FAILED$NC"
fi


EXPECTED="double function() {\nint a = 1, b = 5;\nreturn a + b;\n}"
tst=`diff test.mut.2.1_6_9.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\nint a = (-1), b = 5;\nreturn a + b;\n}"
tst=`diff test.mut.2.2_1_9.ICR.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = (-1);\nreturn a + b;\n}"
tst=`diff test.mut.2.2_7_16.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 0;\nreturn a + b;\n}"
tst=`diff test.mut.2.3_11_16.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi

EXPECTED="double function() {\nint a = 0, b = 5;\nreturn a + b;\n}"
tst=`diff test.mut.2.3_5_9.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi

EXPECTED="double function() {\nint a = (4 + 1), b = 5;\nreturn a + b;\n}"
tst=`diff test.mut.2.4_2_9.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi

EXPECTED="double function() {\nint a = 4, b = (5 + 1);\nreturn a + b;\n}"
tst=`diff test.mut.2.4_8_16.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\nint a = (4 - 1), b = 5;\nreturn a + b;\n}"
tst=`diff test.mut.2.5_3_9.ICR.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = (5 - 1);\nreturn a + b;\n}"
tst=`diff test.mut.2.5_9_16.ICR.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi

EXPECTED="double function() {\nint a = 4, b = -(5);\nreturn a + b;\n}"
tst=`diff test.mut.2.6_10_16.ICR.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\nint a = -(4), b = 5;\nreturn a + b;\n}"
tst=`diff test.mut.2.6_4_9.ICR.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ICR PASSED$NC"
else
    echo -e "${RED}TEST many ICR FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\n;\n}"
tst=`diff test.mut.3.1_1_1.SDL.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many SDL PASSED$NC"
else
    echo -e "${RED}TEST many SDL FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn -(a) + b;\n}"
tst=`diff test.mut.3.1_1_8.ABS.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ABS PASSED$NC"
else
    echo -e "${RED}TEST many ABS FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn  b;\n}"
tst=`diff test.mut.3.1_1_8.AOD.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many AOD PASSED$NC"
else
    echo -e "${RED}TEST many AOD FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a + -(b);\n}"
tst=`diff test.mut.3.1_2_12.ABS.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many ABS PASSED$NC"
else
    echo -e "${RED}TEST many ABS FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn (--a) + b;\n}"
tst=`diff test.mut.3.1_2_8.UOI.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a + (--b);\n}"
tst=`diff test.mut.3.1_6_12.UOI.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a ;\n}"
tst=`diff test.mut.3.2_2_10.AOD.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many AOD PASSED$NC"
else
    echo -e "${RED}TEST many AOD FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn (a--) + b;\n}"
tst=`diff test.mut.3.2_4_8.UOI.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a + (b--);\n}"
tst=`diff test.mut.3.2_8_12.UOI.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn (++a) + b;\n}"
tst=`diff test.mut.3.3_1_8.UOI.function.c <(echo -e "$EXPECTED") | wc -l`
if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi


EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a + (++b);\n}"
tst=`diff test.mut.3.3_5_12.UOI.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn (a++) + b;\n}"
tst=`diff test.mut.3.4_3_8.UOI.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a + (b++);\n}"
tst=`diff test.mut.3.4_7_12.UOI.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST many UOI PASSED$NC"
else
    echo -e "${RED}TEST many UOI FAILED$NC"
fi





