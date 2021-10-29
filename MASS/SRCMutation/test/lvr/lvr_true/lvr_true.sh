#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators LVR

EXPECTED="#include <stdbool.h>\ndouble function() {\nreturn false;\n}"
tst=`diff test.mut.3.4_1_8.LVR.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST lvr_true false PASSED$NC"
else
    echo -e "${RED}TEST lvr_true false FAILED$NC"
fi
