#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LVR

EXPECTED="#include <stdbool.h>\ndouble function() {\nreturn true;\n}"
tst=`diff test.mut.3.5_1_8.LVR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST lvr_false true PASSED$NC"
else
    echo -e "${RED}TEST lvr_false true FAILED$NC"
fi
