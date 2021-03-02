#!/bin/bash

# detect equivalent mutants
source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/original_mutant.sh $MUTATION_DIR/test_runs/all_live

# filter repeated entries on $APP_RUN_DIR/DETECTION/test_runs/main.csv
$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/filter_repeated_test_cases.py $APP_RUN_DIR/DETECTION/test_runs/main.csv

# filter equivalent
$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/filter_equivalent.py $APP_RUN_DIR/DETECTION/test_runs/filtered_main.csv

comm -13 <(sort $APP_RUN_DIR/DETECTION/test_runs/equivalents_distance_zero) <(sort $MUTATION_DIR/test_runs/all_live) > $MUTATION_DIR/test_runs/filtered_live

# backup useful files
mkdir -p $APP_RUN_DIR/RESULTS
cp $APP_RUN_DIR/DETECTION/test_runs/useful_list_* $APP_RUN_DIR/RESULTS
cp $MUTATION_DIR/test_runs/all_killed $APP_RUN_DIR/RESULTS
cp $MUTATION_DIR/test_runs/filtered_live $APP_RUN_DIR/RESULTS

# estimate mutation score
killed=$(cat $MUTATION_DIR/test_runs/all_killed | wc -l)
live=$(cat $MUTATION_DIR/test_runs/filtered_live | wc -l)

MASS_ms=$(echo "scale=3; ${killed}/(${killed}+${live})" | bc -l)

echo "Total live mutants: $live"
echo "Total killed mutants: $killed"
echo "MASS mutation score: $MASS_ms"



