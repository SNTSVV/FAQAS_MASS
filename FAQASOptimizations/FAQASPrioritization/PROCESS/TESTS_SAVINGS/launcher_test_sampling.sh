
PRIORITIZATION=$FAQAS/GSL/PRIORITIZATION
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/LIBUTIL
TOT=37

ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS/TESTS_SAVINGS/process_nr_test_sampling.py

python3.6 $PROCESS $WORKING $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT

