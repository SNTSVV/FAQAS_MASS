#!/bin/bash

RESULTS=$FAQAS/CSP_GSL/MUTATION/runs/all_traces.csv
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/CSP_GSL

FILTER_SCRIPT=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/filter_red_eq_stubborn.py

# creating a temporary file with equivalent and redundants
pushd $WORKING
python3.6 $FILTER_SCRIPT eq_red $RESULTS
popd

