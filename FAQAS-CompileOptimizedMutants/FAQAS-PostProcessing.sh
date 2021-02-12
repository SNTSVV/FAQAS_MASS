#!/bin/bash

RESULTS=$1

shopt -s extglob
for level_dir in $RESULTS/*; do
    level=$(basename $level_dir)
    echo "Processing optimisation level $level ..."

    $MASS/FAQAS-CompileOptimizedMutants/tce_process.py $level_dir/hashes.csv $level_dir/original_hash
    mv all all_$level
    mv equivalents equivalents_$level
    mv redundants redundants_$level
done

cat all_* | sort | uniq > all
cat equivalents_* | sort | uniq > equivalents
cat redundants_* | sort | uniq > redundants

rm all_* equivalents_* redundants_*

# remove redundants and equivalents from all mutants list
cat redundants equivalents | sort | uniq | comm -13 - <(sort all) > all_filtered

mv all_filtered all equivalents redundants $RESULTS
