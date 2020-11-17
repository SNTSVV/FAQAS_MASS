#!/bin/bash

RESULTS=$FAQAS/PARAM_GSL/MUTATION/runs/all_traces.csv
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/PARAM_GSL

FILTER_SCRIPT=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/filter_red_eq_stubborn.py

# creating a temporary file with equivalent and redundants
pushd $WORKING
cat equivalents_* redundants_* | sort | uniq > eq_red
python3.6 $FILTER_SCRIPT eq_red $RESULTS
popd

