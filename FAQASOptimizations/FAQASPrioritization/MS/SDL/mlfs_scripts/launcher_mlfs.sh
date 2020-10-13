
PRIORITIZATION=$FAQAS/MLFS/PRIORITIZATION
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/MLFS

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/MS/mlfs_scripts/process_ms_wo_location.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER "/home/mlfs/unit-test-suite/" 

