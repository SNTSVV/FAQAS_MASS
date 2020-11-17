#!/bin/bash
PROJ=CSP

#########
TO_PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/$PROJ/REG
CALCULATOR=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/calculator.py

python3.6 $CALCULATOR $TO_PROCESS
#mv simulations.csv all_sdl_reg.csv


