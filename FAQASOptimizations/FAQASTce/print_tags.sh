#!/bin/bash

SRC_MUTANTS=/tmp/ESAIL/runs/run_0/src-mutants/
COMPILED=OBSW.exe

find $SRC_MUTANTS -name "*.${COMPILED}" | wc -l
find $SRC_MUTANTS -name '*.equivalent' | wc -l
find $SRC_MUTANTS -name '*.redundant' | wc -l                                                                          
find $SRC_MUTANTS -name '*.notcompiled' | wc -l
