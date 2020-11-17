
PRIORITIZATION=$FAQAS/CSP_GSL/PRIORITIZATION
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/CSP_GSL
TOT=14

ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS_ONLY_COVERED/process_time_sampling.py

python3.6 $PROCESS $WORKING $PRIORITIZATION $ORIGINAL_ORDER "/home/csp/libparam/lib/libgscsp/tst/" $TOT

