#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LCR

EXPECTED="double function() {\nint a = 1, b;\nb &= a;\nreturn b;\n}"
tst_and=`diff test.mut.3.3_1_3.LCR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nint a = 1, b;\nb ^= a;\nreturn b;\n}"
tst_xor=`diff test.mut.3.5_2_3.LCR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_and -eq 0 ];then
    echo -e "${GREEN}TEST lcr_or_assign and PASSED$NC"
else
    echo -e "${RED}TEST lcr_or_assign and FAILED$NC"
fi                                                                                                                                    

if [ $tst_xor -eq 0 ];then
    echo -e "${GREEN}TEST lcr_or_assign xor PASSED$NC"
else
    echo -e "${RED}TEST lcr_or_assign xor FAILED$NC"
fi                                                                                                                                    
