#!/bin/bash
PROJ=ESAIL

#########
TO_PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/$PROJ/FIXED
CALCULATOR=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/NEW_SAMPLING/calculator_fixed.py

python3.7 $CALCULATOR $TO_PROCESS


