#!/bin/bash

#########
TO_PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/MLFS/FIXED
CALCULATOR=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/RQ5/calculator_fixed_wo_location.py

PRIORITIZATION=$FAQAS/MLFS/PRIORITIZATION
ORIGINAL_ORDER=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/MLFS/test_cases
TOT=94
LIST=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASMutationScore/RQ5/mlfs_list


python3.6 $CALCULATOR $TO_PROCESS $PRIORITIZATION $ORIGINAL_ORDER "/home/mlfs/unit-test-suite/" $TOT $LIST

