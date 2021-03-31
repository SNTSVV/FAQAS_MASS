#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LOD

EXPECTED="double function() {\nint a = 4, b = 5;\nif (( b > 0) || 1)\nreturn 1;\nelse\nreturn 0;\n}"
tst_a=`diff test.mut.3.1_2_6.LOD.function.c <(echo -e "$EXPECTED") | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nif ((a > 0 && b > 0) )\nreturn 1;\nelse\nreturn 0;\n}"
tst_b=`diff test.mut.3.2_4_22.LOD.function.c <(echo -e "$EXPECTED") | wc -l`
                                                                                                                                      
if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST lod_many 1 PASSED$NC"
else
    echo -e "${RED}TEST lod_many 1 FAILED$NC"
fi

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST lod_many 2 PASSED$NC"
else
    echo -e "${RED}TEST lod_many 2 FAILED$NC"
fi

EXPECTED="double function() {\nint a = 4, b = 5;\nif ((a > 0 ) || 1)\nreturn 1;\nelse\nreturn 0;\n}"
tst_a=`diff test.mut.3.2_3_12.LOD.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nif ( 1)\nreturn 1;\nelse\nreturn 0;\n}"
tst_b=`diff test.mut.3.1_1_5.LOD.function.c <(echo -e "$EXPECTED") | wc -l`
                                                                                                                                      
if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST lod_many 3 PASSED$NC"
else
    echo -e "${RED}TEST lod_many 3 FAILED$NC"
fi

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST lod_many 4 PASSED$NC"
else
    echo -e "${RED}TEST lod_many 4 FAILED$NC"
fi

