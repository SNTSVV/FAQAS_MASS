#!/bin/bash

TO_PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/MLFS_FIXED
CALCULATOR=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/calculator_fixed.py

python3.6 $CALCULATOR $TO_PROCESS

