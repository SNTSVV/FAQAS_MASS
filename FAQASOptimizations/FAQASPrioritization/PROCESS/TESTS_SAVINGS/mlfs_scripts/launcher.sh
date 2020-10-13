
PRIORITIZATION=$FAQAS/MLFS/PRIORITIZATION

#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/MLFS
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/MLFS
#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/PARAM_GSL

TOT=94

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS/TESTS_SAVINGS/mlfs_scripts/process_nr_test.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER "/home/mlfs/unit-test-suite/" $TOT
#python3.6 $PROCESS $ALL_KILLED $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT
#python3.6 $PROCESS $ALL_LIVE $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT

