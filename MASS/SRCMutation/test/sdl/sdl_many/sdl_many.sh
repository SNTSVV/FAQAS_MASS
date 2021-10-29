#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators SDL

EXPECTED="void function() {\n\nreturn;\n}"

tst=`diff test.mut.2.1_1_1.SDL.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST sdl PASSED$NC"
else
    echo -e "${RED}TEST sdl FAILED$NC"
fi

EXPECTED="void function() {\nint a = 0;\n;\n}"

tst_2=`diff test.mut.3.1_1_1.SDL.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst_2 -eq 0 ];then
    echo -e "${GREEN}TEST sdl_2 PASSED$NC"
else
    echo -e "${RED}TEST sdl_2 FAILED$NC"
fi
