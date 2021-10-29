#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators SDL

EXPECTED="void function() {\n\n}"

tst=`diff test.mut.2.1_1_1.SDL.function.c <(echo -e $EXPECTED) | wc -l`

if [ $tst -eq 0 ];then
    echo -e "${GREEN}TEST sdl PASSED$NC"
else
    echo -e "${RED}TEST sdl FAILED$NC"
fi
