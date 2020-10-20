
PRIORITIZATION=$FAQAS/MLFS/PRIORITIZATION

ORIGINAL_COV=$FAQAS/MLFS/COVERAGE_TESTS_MATCH

WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/MLFS
#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/MLFS

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS_ONLY_COVERED/mlfs_scripts/process_time.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER "/home/mlfs/unit-test-suite/" $ORIGINAL_COV

