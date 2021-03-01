#!/bin/bash

# processing equivalent mutants

# detect equivalent mutants
#source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/original_mutant.sh $MUTATION_DIR/test_runs/all_live

# filter repeated entries on $APP_RUN_DIR/DETECTION/test_runs/main.csv
#$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/filter_repeated_test_cases.py $APP_RUN_DIR/DETECTION/test_runs/main.csv

# filter equivalent
$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/filter_equivalent.py $APP_RUN_DIR/DETECTION/test_runs/filtered_main.csv $MUTATION_DIR/test_runs/all_live
