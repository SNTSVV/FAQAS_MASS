
PRIORITIZATION=$SCRATCH/ESAIL/PRIORITIZATION
WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASSampling/RQ5/ESAIL

ORIGINAL_ORDER=$WORKING/test_cases

PROCESS=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASPrioritization/PROCESS_ONLY_COVERED/TESTS_SAVINGS/process_test_sampling_esail.py

python3.6 $PROCESS $WORKING $PRIORITIZATION $ORIGINAL_ORDER "" 

