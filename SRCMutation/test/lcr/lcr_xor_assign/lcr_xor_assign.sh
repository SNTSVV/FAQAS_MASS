#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LCR

EXPECTED="double function() {\nint a = 1, b;\nb &= a;\nreturn b;\n}"
tst_and=`diff test.mut.3.3_1_3.LCR.function.c <(echo -e $EXPECTED) | wc -l`

EXPECTED="double function() {\nint a = 1, b;\nb |= a;\nreturn b;\n}"
tst_or=`diff test.mut.3.4_2_3.LCR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_and -eq 0 ];then
    echo -e "${GREEN}TEST lcr_xor_assign and PASSED$NC"
else
    echo -e "${RED}TEST lcr_xor_assign and FAILED$NC"
fi                                                                                                                                    

if [ $tst_or -eq 0 ];then
    echo -e "${GREEN}TEST lcr_xor_assign or PASSED$NC"
else
    echo -e "${RED}TEST lcr_xor_assign or FAILED$NC"
fi                                                                                                                                    
