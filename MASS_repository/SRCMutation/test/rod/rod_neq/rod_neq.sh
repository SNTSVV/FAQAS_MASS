#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators ROD

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn a ;\n}"
tst_a=`diff test.mut.3.2_2_10.ROD.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nint a = 4, b = 5;\nreturn  b;\n}"                                                             
tst_b=`diff test.mut.3.1_1_8.ROD.function.c <(echo -e "$EXPECTED") | wc -l`

if [ $tst_a -eq 0 ];then
    echo -e "${GREEN}TEST rod_neq a PASSED$NC"
else
    echo -e "${RED}TEST rod_neq a FAILED$NC"
fi                                                                                                                                    

if [ $tst_b -eq 0 ];then
    echo -e "${GREEN}TEST rod_neq b PASSED$NC"
else
    echo -e "${RED}TEST rod_neq b FAILED$NC"
fi                                                                                                                                    

