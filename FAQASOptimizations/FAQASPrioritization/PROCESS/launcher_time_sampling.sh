
PRIORITIZATION=$FAQAS/PARAM_GSL/PRIORITIZATION
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/PARAM_GSL
TOT=13

ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS/process_time_sampling.py

python3.6 $PROCESS $WORKING $PRIORITIZATION $ORIGINAL_ORDER "/home/csp/libparam/tst/" $TOT

