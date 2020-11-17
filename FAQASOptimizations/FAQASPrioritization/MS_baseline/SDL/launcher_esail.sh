PRIORITIZATION=$FAQAS/ESAIL/COVERAGE_TESTS_MATCH/sources                                                                             
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/ESAIL

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/MS_baseline/process_ms.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER ""
