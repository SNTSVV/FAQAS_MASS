
PRIORITIZATION=$FAQAS/MLFS/PRIORITIZATION

WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/MLFS
TOT=94

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS_ONLY_COVERED/TESTS_SAVINGS/mlfs_scripts/process_nr_test_sampling.py

python3.6 $PROCESS $WORKING $PRIORITIZATION $ORIGINAL_ORDER "/home/mlfs/unit-test-suite/" $TOT

