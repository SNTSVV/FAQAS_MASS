
PRIORITIZATION=$FAQAS/CSP_GSL/PRIORITIZATION

#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/PARAM_GSL
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/CSP_GSL
#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/PARAM_GSL

TOT=14

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS/TESTS_SAVINGS/process_nr_test.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER "/home/csp/libparam/lib/libgscsp/tst/" $TOT
#python3.6 $PROCESS $ALL_KILLED $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT
#python3.6 $PROCESS $ALL_LIVE $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT

