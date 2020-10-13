
PRIORITIZATION=$FAQAS/GSL/PRIORITIZATION

#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/LIBUTIL
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL/LIBUTIL
#WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/PARAM_GSL

TOT=37

ALL_TRACES=$WORKING/all_traces_filtered.csv
ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS/process_time.py

python3.6 $PROCESS $ALL_TRACES $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT
#python3.6 $PROCESS $ALL_KILLED $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT
#python3.6 $PROCESS $ALL_LIVE $PRIORITIZATION $ORIGINAL_ORDER "/home/gsl/libutil/tst/" $TOT

