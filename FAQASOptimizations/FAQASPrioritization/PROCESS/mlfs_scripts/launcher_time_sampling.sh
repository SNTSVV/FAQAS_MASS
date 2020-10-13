
PRIORITIZATION=$FAQAS/MLFS/PRIORITIZATION
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/MLFS
TOT=94

ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS/mlfs_scripts/process_time_sampling.py

python3.6 $PROCESS $WORKING $PRIORITIZATION $ORIGINAL_ORDER "/home/mlfs/unit-test-suite/" $TOT

