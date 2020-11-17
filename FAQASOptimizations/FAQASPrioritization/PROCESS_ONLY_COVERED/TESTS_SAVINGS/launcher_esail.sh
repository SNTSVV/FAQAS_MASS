
PRIORITIZATION=$SCRATCH/ESAIL/PRIORITIZATION

ORIGINAL_COV=$FAQAS/ESAIL/COVERAGE_TESTS_MATCH

#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL

WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/ESAIL

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS_ONLY_COVERED/TESTS_SAVINGS/process_test_esail.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER "" $ORIGINAL_COV

