#!/bin/bash

SRC_MUTANTS=/opt/mutations/src-mutants
COMPILED=OBSW.exe

find $SRC_MUTANTS -name "*.${COMPILED}" -delete
find $SRC_MUTANTS -name '*.equivalent' -delete
find $SRC_MUTANTS -name '*.redundant' -delete                                                                                                 
find $SRC_MUTANTS -name '*.notcompiled' -delete
