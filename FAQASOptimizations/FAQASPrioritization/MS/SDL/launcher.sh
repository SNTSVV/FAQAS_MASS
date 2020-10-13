
PRIORITIZATION=$FAQAS/PARAM_GSL/PRIORITIZATION
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/PARAM_GSL

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/MS/process_ms.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER "/home/csp/libparam/tst/"
